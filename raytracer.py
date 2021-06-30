import sys
import math
import glm
from PIL import Image
import concurrent.futures
from multiprocessing import Pool

class Light():
    def __init__(self, lightType, position, color):
        self.lightType = lightType
        self.position = position
        self.color = color

    # return (light_vector, light_color) tuple
    def getIllumination(self, vpos):
        lightDir = None
        lightColor = self.color

        if self.lightType == 'directional':
            lightDir = -glm.normalize(self.position)
        elif self.lightType == 'point':
            lightDir = glm.normalize(self.position - vpos)
        else:
            print("unknown light type")
            raise KeyError

        return (lightDir, lightColor)

class Material:
    def __init__(self, *args):
        self.diffuse = None
        self.specular = None
        self.emission = None
        self.shininess = None

        if len(args) == 0:
            return
        if len(args) == 4:
            self.diffuse = args[0]
            self.specular = args[1]
            self.emission = args[2]
            self.shininess = args[3]

    def setDiffuse(self, dcolor):
        self.diffuse = dcolor

    def setSpecular(self, scolor):
        self.specular = scolor

    def setShininess(self, shininess):
        self.shininess = shininess

    def setEmission(self, emission):
        self.emission = emission

    # return color
    def shade(self, ray, pos, normal, light):
        output = glm.vec3(0.0)

        nlightDirection, lightColor = light.getIllumination(pos)
        nnormal = glm.normalize(normal)

        diffuseIntensity = glm.dot(nnormal, nlightDirection)
        output = output + self.diffuse * lightColor * glm.clamp(diffuseIntensity, 0.0, 1.0)

        nview = glm.normalize(ray.origin - pos)
        nhalf = glm.normalize(nview + nlightDirection)

        specularIntensity = glm.pow(glm.dot(nhalf, nnormal), self.shininess)
        output = output + self.specular * lightColor * glm.clamp(specularIntensity, 0.0, 1.0)

        output = output + self.emission

        return glm.vec3(
                glm.clamp(output.x, 0.0, 1.0),
                glm.clamp(output.y, 0.0, 1.0),
                glm.clamp(output.z, 0.0, 1.0))
        #return glm.vec3(0.3, 1.0, 0.8)

class Ray:
    def __init__(self, origin, direction):
        self.origin = origin
        self.direction = direction

    def pointAt(self, t):
        return self.origin + t * glm.normalize(self.direction)

class Camera:
    def __init__(self, eye, forward, up, fovy):
        self.eye = eye
        self.forward = glm.normalize(forward)
        self.horizontal = glm.normalize(glm.cross(self.forward, up))
        self.up = glm.normalize(glm.cross(self.horizontal, self.forward))

        self.fovy2 = fovy / 2.0
        self.fovx2 = self.fovy2

    def setAspectRatio(self, aspectRatio):
        self.fovx2 = self.fovy2 * aspectRatio

    def generateRay(self, pt):
        alpha = math.tan(self.fovx2) * pt.x
        beta = math.tan(self.fovy2) * pt.y
        rayDirection = self.horizontal * alpha + self.up * beta + self.forward
        return Ray(self.eye, rayDirection)

    def getTMin(self):
        return 0.0

class Hit:
    def __init__(self, t, normal, material):
        self.t = t
        self.material = material
        self.normal = normal

class Primitive:
    def __init__(self, material):
        self.material = material
    # return hit/None
    def intersect(self, ray, hit, tmin):
        raise KeyError

class Sphere:
    def __init__(self, center, radius, material):
        self.center = center
        self.radius = radius
        self.material = material

    def intersect(self, ray, hit, tmin):
        oc = ray.origin - self.center
        oc2 = glm.dot(oc, oc)

        direction = glm.normalize(ray.direction)

        d2 = glm.dot(direction, direction)
        ocDotd = glm.dot(oc, direction)

        disc = (ocDotd * ocDotd) - d2 * (oc2 - self.radius * self.radius)
        if disc < 0:
            return None

        discRoot = math.sqrt(disc)
        t1 = (-ocDotd - discRoot) / d2
        t2 = (-ocDotd + discRoot) / d2

        t = max(min(t1, t2), 0.0)
        if t <= tmin:
            return None
        if (not hit) or (t < hit.t):
            pt = ray.pointAt(t)
            normal = pt - self.center
            return Hit(t, normal, self.material)
        return None

class Transform(Primitive):
    def __init__(self, primitive, matrix):
        self.primitive = primitive
        self.matrix = matrix

    def intersect(self, ray, hit, tmin):
        minverse = glm.inverse(self.matrix)

        tdir4 = minverse * glm.vec4(ray.direction, 0.0)
        tdir3 = tdir4.xyz
        if tdir4.w != 0.0:
            tdir3 = tdir3 / tdir4.w

        torig4 = minverse * glm.vec4(ray.origin, 1.0)
        torig3 = torig4.xyz / torig4.w

        hit = self.primitive.intersect(Ray(torig3, tdir3), hit, tmin)
        if hit:
            hit.normal = (glm.transpose(minverse) * glm.vec4(hit.normal, 1.0)).xyz

        return hit

class Group(Primitive):
    def __init__(self):
        self.primitives = []

    def addPrimitive(self, primitive):
        self.primitives.append(primitive)

    def intersect(self, ray, hit, tmin):
        hit = None

        for primitive in self.primitives:
            newHit = primitive.intersect(ray, hit, tmin)
            if newHit and ((not hit) or (hit.t > newHit.t)):
                hit = newHit

        return hit

class Scene:
    def __init__(self):
        self.imgFilename = None
        self.imgSize = None
        self.image = None

        self.camera = None

        self.matrixStack = [glm.mat4(1.0)]
        self.currentMatrix = glm.mat4(1.0)

        self.lights = []

        self.group = Group()
        self.ambient = None
        self.currentMaterial = Material()

        self.backgroundColor = glm.vec3(0.7, 0.7, 0.7)

    def parse(self, filename):
        flr = open(filename)
        for line in flr.readlines():
            line = line[:-1]
            line.lstrip().rstrip()
            if len(line) == 0 or line[0] == '#':
                continue
            fields = line.split(' ')
            if fields[0] == 'size':
                self.imgSize = glm.vec2(int(fields[1]), int(fields[2]))
                if self.camera:
                    self.camera.setAspectRatio(self.imgSize.x / self.imgSize.y)
                continue

            if fields[0] == 'output':
                self.imgFilename = fields[1]
                continue

            if fields[0] == 'camera':
                eye = glm.vec3(float(fields[1]), float(fields[2]), float(fields[3]))
                forward = glm.vec3(float(fields[4]), float(fields[5]), float(fields[6]))
                up = glm.vec3(float(fields[7]), float(fields[8]), float(fields[9]))
                degrees = float(fields[10])
                fovy = degrees * math.pi / 180.0
                self.camera = Camera(eye, forward, up, fovy)
                if self.imgSize:
                    self.camera.setAspectRatio(self.imgSize.x / self.imgSize.y)
                continue

            if fields[0] == 'point' or fields[0] == 'directional':
                lpos = glm.vec3(float(fields[1]), float(fields[2]), float(fields[3]))
                lcol = glm.vec3(float(fields[4]), float(fields[5]), float(fields[6]))
                self.lights.append(Light(fields[0], lpos, lcol))
                continue

            if fields[0] == 'ambient':
                self.ambient = glm.vec3(float(fields[1]), float(fields[2]), float(fields[3]))
            if fields[0] == 'specular':
                self.currentMaterial.setSpecular(glm.vec3(float(fields[1]), float(fields[2]), float(fields[3])))
            elif fields[0] == 'diffuse':
                self.currentMaterial.setDiffuse(glm.vec3(float(fields[1]), float(fields[2]), float(fields[3])))
            elif fields[0] == 'shininess':
                self.currentMaterial.setShininess(float(fields[1]))
            elif fields[0] == 'emission':
                self.currentMaterial.setEmission(glm.vec3(float(fields[1]), float(fields[2]), float(fields[3])))

            if fields[0] == 'pushTransform':
                self.matrixStack.append(self.currentMatrix)
                continue
            if fields[0] == 'popTransform':
                self.currentMatrix = self.matrixStack.pop()
                continue

            if fields[0] == 'translate':
                tvec = glm.vec3(float(fields[1]), float(fields[2]), float(fields[3]))
                self.currentMatrix = glm.translate(self.currentMatrix, tvec)
                continue
            if fields[0] == 'rotate':
                axis = glm.vec3(float(fields[1]), float(fields[2]), float(fields[3]))
                angle = float(fields[4]) * math.pi / 180.0
                self.currentMatrix = glm.rotate(self.currentMatrix, angle, axis)
                continue
            if fields[0] == 'scale':
                svec = glm.vec3(float(fields[1]), float(fields[2]), float(fields[3]))
                self.currentMatrix = glm.scale(self.currentMatrix, svec)
                continue

            if fields[0] == 'sphere':
                center = glm.vec3(float(fields[1]), float(fields[2]), float(fields[3]))
                radius = float(fields[4])
                sphere = Sphere(center, radius, self.currentMaterial)
                self.group.addPrimitive(Transform(sphere, self.currentMatrix))
        return

def trace(w, h, width, height, scene):
    i = (height / 2.0 - h) / (height / 2.0)
    j = (w - width / 2.0) / (width / 2.0)

    ray = scene.camera.generateRay(glm.vec2(j, i))

    hit = scene.group.intersect(ray, None, scene.camera.getTMin())

    color = glm.vec3(0.0)
    if hit:
        for light in scene.lights:
            contrib = hit.material.shade(ray, ray.pointAt(hit.t), hit.normal, light)
            color = color + contrib
        color = color + scene.ambient
    else:
        color = scene.backgroundColor

    intc = (min(int(color.r * 256), 255), min(int(color.g * 256), 255), min(int(color.b * 256), 255))
    return (w, h, intc)

def main(sceneFilename):
    scene = Scene()
    scene.parse(sceneFilename)

    width = int(scene.imgSize.x)
    height = int(scene.imgSize.y)

    image = Image.new(mode='RGB', size=(width, height))

    inarr = []
    outarr = []

    for w in range(width):
        for h in range(height):
            inarr.append((w, h))

    ## parallel processing
    ####with concurrent.futures.ProcessPoolExecutor() as executor:
    ####    outarr = executor.map(lambda tp: trace(tp[0], tp[1], width, height, scene), inarr)

    ## sequential processing
    ####for w, h in inarr:
    ####    outarr.append(trace(w, h, width, height, scene))

    ## using multiprocessing
    pool = Pool(processes=4)
    outarr = [pool.apply_async(trace, (w, h, width, height, scene)) for w, h in inarr]

    for out in outarr:
        w, h, intc = out.get()
        image.putpixel((w, h), intc)

    image.save(scene.imgFilename)

    return

if __name__ == '__main__':
    main(sys.argv[1])
