#include "RayTracer.h"

double RayTracer::jitter(int i, double scale) {
    uniform_real_distribution<double> unif(0,scale);
    default_random_engine re;
    return i + unif(re);
}

Ray RayTracer::computeRay(double i, double j, double angle, double aspectratio, Environment* env) {
    double xx = (2 * (i * 1/PLACE) - 1) * angle * aspectratio;
    double yy = (1 - 2 * (j * 1/PLACE)) * angle;
    Position look_from = env->from;
    Position look_at = Position(xx,yy,0) + env->at;

    Ray ray = Ray(look_from, look_at);
    return ray;
}

Ray RayTracer::jitter(Ray ray) {
    double scale = 0.025;
    ray.rd = Direction(Position(jitter(ray.rd.x, scale),jitter(ray.rd.y, scale),jitter(ray.rd.z, scale)));
    return ray;
}

Colori RayTracer::trace(Ray ray, Environment* env) {
    Position p = Position(numeric_limits<double>::infinity());
    Object* closest = NULL;

    for(Object* o: env->env) {
        Position np = o->findIntersect(ray);
        if(length(np) < length(p)) {
            closest = o;
            p = np;
        }
    }
    
    if (!closest)
        return BACKGROUND;
    else {
        bool blocked = false;
        Light light = { env->light_color, env->light_position };
        Direction l = light.position - p;
        Ray toLight(p, l);
        double distance = length(light.position - p);
        for (Object* o: env->env) {
            if (length(light.position - o->findIntersect(toLight)) < distance) {
                blocked = true;
                break;
            }
        }
        if (closest->getMaterial()->isReflective) {
            Ray newRay(p, closest->computeNormal(p));
            Colori ci = closest->computeColor(ray.r0, p, light, false);
            Colord cd = Colord(ci.x/255.,ci.y/255.,ci.z/255.);
            Colori ct = Colori();
            for(int y = 0; y < 4; y++) {
                ct = ct + trace(newRay, env);
            }
            ct = ct / 4;
            return Colori(ct.x*cd.x,ct.y*cd.y,ct.z*cd.z);
        }
        else
            return closest->computeColor(ray.r0, p, light, blocked);
    }
}

vector<Colori> RayTracer::subdivide(int i, int j, double scale, double angle, double aspectratio, Environment* env) {
    vector<Colori> colors = vector<Colori>();
    for(int y = 0; y < scale; y++) {
        for(int x = 0; x < scale; x++) {
            colors.push_back(trace(computeRay(jitter(i, 1 / scale) + x / scale, jitter(j, 1 / scale) + y / scale, angle, aspectratio, env), env));
        }
    }
    return colors;
}