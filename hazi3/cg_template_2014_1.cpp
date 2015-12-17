//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization függvényt kivéve, a lefoglalt adat korrekt felszabadítása nélkül 
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:  
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D, 
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi, 
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    :
// Neptun :
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy 
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem. 
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a 
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb 
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem, 
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.  
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat 
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>


#if defined(__APPLE__)                                                                                                                                                                                                            
#include <OpenGL/gl.h>                                                                                                                                                                                                            
#include <OpenGL/glu.h>                                                                                                                                                                                                           
#include <GLUT/glut.h>                                                                                                                                                                                                            
#else                                                                                                                                                                                                                             
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)                                                                                                                                                                       
#include <windows.h>                                                                                                                                                                                                              
#endif                                                                                                                                                                                                                            
#include <GL/gl.h>                                                                                                                                                                                                                
#include <GL/glu.h>                                                                                                                                                                                                               
#include <GL/glut.h>                                                                                                                                                                                                              
#endif          


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

#define PI 3.141592

struct Vector {
   float x, y, z;

   Vector( ) { 
	x = y = z = 0;
   }
   Vector(float x0, float y0, float z0 = 0) { 
	x = x0; y = y0; z = z0;
   }
   Vector operator*(float a) { 
	return Vector(x * a, y * a, z * a); 
   }
   Vector operator+(const Vector& v) {
 	return Vector(x + v.x, y + v.y, z + v.z); 
   }
   Vector operator-(const Vector& v) {
 	return Vector(x - v.x, y - v.y, z - v.z); 
   }
   float operator*(const Vector& v) {
	return (x * v.x + y * v.y + z * v.z); 
   }
   Vector operator%(const Vector& v) {
	return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
   }
   Vector operator/(float a) {
	   return Vector(x / a, y / a, z / a);
   }
   float Length() { return sqrt(x * x + y * y + z * z); }
   Vector normalize() {
	   return (*this) / (*this).Length();
   }
};
 
struct Color {
   float r, g, b;

   Color( ) { 
	r = g = b = 0;
   }
   Color(float r0, float g0, float b0) { 
	r = r0; g = g0; b = b0;
   }
   Color operator*(float a) { 
	return Color(r * a, g * a, b * a); 
   }
   Color operator*(const Color& c) { 
	return Color(r * c.r, g * c.g, b * c.b); 
   }
   Color operator+(const Color& c) {
 	return Color(r + c.r, g + c.g, b + c.b); 
   }
};

const int screenWidth = 600;
const int screenHeight = 600;

Vector gravity(0.0, -5.0, 0.0);

class Object {
	bool hasShadow;
public:
	Object(bool s) : hasShadow(s) {

	}

	void setHasshadow(bool h) {
		hasShadow = h;
	}

	virtual void draw() = 0;

	virtual void setRotation(float x, float y, float z) = 0;

	virtual void setScale(float x, float y, float z) {
	}

	virtual void inverseKinematics(float dx, float dy) {
	}

	virtual void setPosition(Vector p) {

	}

	virtual void blast(Vector c) {

	}

	virtual float getDistance(Vector c) = 0;

	virtual float getHeight() {
		return 0;
	}

	bool shadow() {
		return hasShadow;
	}

	virtual void animate(float time) = 0;
};

struct ObjectList {
	Object* obj;
	ObjectList* next;

	ObjectList(Object* o) : obj(o), next(NULL) {

	}

	~ObjectList() {

	}
};

class ObjectHolder {
	ObjectList* objects;
	int objNum;
public:
	ObjectHolder() : objects(NULL), objNum(0) {

	}

	void addObject(Object* o) {
		ObjectList* ol = new ObjectList(o);
		if (objects == NULL) {
			objects = ol;
		}
		else {
			ObjectList* current = objects;
			while (current->next != NULL) {
				current = current->next;
			}
			current->next = ol;
		}
		++objNum;
	}

	int getObjNum() {
		return objNum;
	}

	Object* getObject(int pos) {
		if (objNum <= pos) {
			return NULL;
		}
		ObjectList* current = objects;
		for (int i = 0; i < pos; ++i) {
			current = current->next;
		}
		return current->obj;
	}

	void deleteObjAt(int n) {
		ObjectList* remove;
		ObjectList* prev;
		if (n == 0) {
			ObjectList* remove = objects;
			objects = remove->next;
			delete remove;
			objNum--;
			return;
		}
		prev = objects;
		for (int i = 0; i < n-1; ++i) {
			prev = prev->next;
		}
		remove = prev->next;
		prev->next = remove->next;
		delete remove;
		objNum--;
	}

	~ObjectHolder() {
		ObjectList* current = objects;
		while (current != NULL) {
			ObjectList* del = current;
			current = current->next;
			delete del;
		}
	}
};

class Material {
	Color kd;
	Color ks;
	Color ka;
	float shininess;
	float colA;
	float shineA;
public:
	Material(Color d, Color s, Color a, float shiny, float cA, float sA) : kd(d), ks(s), ka(a), shininess(shiny), colA(cA), shineA(sA) {

	}

	void setOGL() {
		float col[4] = { kd.r, kd.g, kd.b, colA };
		float shine[4] = { ks.r, ks.g, ks.b, shineA };
		float ambient[4] = { ka.r, ka.g, ka.b, 1 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
		glMaterialfv(GL_FRONT, GL_SPECULAR, shine);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}
};

struct PointList {
	Vector point;
	float time;
	PointList* next;

	PointList(Vector p, float t) : point(p), time(t), next(NULL) {

	}

	~PointList() {

	}
};

class PointHolder {
	PointList* points;
	int pointNum;
public:
	PointHolder() : points(NULL), pointNum(0) {

	}
	void addPoint(Vector v, float t) {
		PointList* p = new PointList(v, t);
		if (points == NULL) {
			points = p;
		}
		else {
			PointList* current = points;
			while (current->next != NULL) {
				current = current->next;
			}
			current->next = p;
		}
		++pointNum;
	}

	PointList* getPoint(int n) {
		if (pointNum <= n) {
			return NULL;
		}
		PointList* current = points;
		for (int i = 0; i < n; ++i) {
			current = current->next;
		}
		return current;
	}

	int getPointNum() {
		return pointNum;
	}

	~PointHolder() {
		PointList* current = points;
		PointList* del;
		while (current != NULL) {
			del = current;
			current = current->next;
			delete del;
		}
	}
};

class Spline {
	PointHolder points;

	Vector getSpeed(int pointNum) {
		if (pointNum == 0 || pointNum == (points.getPointNum() - 1)) {
			return Vector(0.0, 0.0, 0.0);
		}
		PointList* p = points.getPoint(pointNum - 1);
		Vector p0 = p->point;
		float t0 = p->time;
		p = p->next;
		Vector p1 = p->point;
		float t1 = p->time;
		p = p->next;
		Vector p2 = p->point;
		float t2 = p->time;
		float v_x = 0.5*(((p2.x - p1.x) / (t2 - t1)) + ((p1.x - p0.x) / (t1 - t0)));
		float v_y = 0.5*(((p2.y - p1.y) / (t2 - t1)) + ((p1.y - p0.y) / (t1 - t0)));
		float v_z = 0.5*(((p2.z - p1.z) / (t2 - t1)) + ((p1.z - p0.z) / (t1 - t0)));
		return Vector(v_x, v_y, v_z);
	}

	Vector hermite(Vector p0, Vector v0, float t0, Vector p1, Vector v1, float t1, float t) {
		Vector a0 = p0;
		Vector a1 = v0;
		Vector a2 = (((p1 - p0) * 3) / (pow(t1 - t0, 2))) - ((v1 + (v0 * 2)) / (t1 - t0));
		Vector a3 = (((p0 - p1) * 2) / (pow(t1 - t0, 3))) + ((v1 + v0) / (pow(t1 - t0, 2)));
		Vector p = a3*(pow(t - t0, 3)) + a2*(pow(t - t0, 2)) + a1*(t - t0) + a0;
		return p;
	}
public:
	Spline() {

	}

	Spline(Spline& s) {
		for (int i = 0; i < s.points.getPointNum(); ++i) {
			PointList* p = s.points.getPoint(i);
			points.addPoint(p->point, p->time);
		}
	}

	void addPoint(Vector p, float t) {
		points.addPoint(p, t);
	}

	Vector getSurfacePoint(float t) {
		for (int i = 0; i < points.getPointNum(); ++i) {
			PointList* p = points.getPoint(i);
			if (p->time <= t && t <= p->next->time) {
				return hermite(p->point, getSpeed(i), p->time, p->next->point, getSpeed(i+1), p->next->time, t);
			}
		}
	}

	
};

class Bezier {
	Vector points[8];

	float B(int i, float t) {
		int n = 9 - 1;
		float choose = 1.0;
		for (int j = 1; j <= i; ++j) {
			choose *= (float)(n - j + 1) / j;
		}
		return choose*pow(t, i)*pow(1 - t, n - i);
	}
public:
	Bezier(Vector p1, Vector p2, Vector p3, Vector p4, Vector p5, Vector p6, Vector p7, Vector p8) {
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
		points[3] = p4;
		points[4] = p5;
		points[5] = p6;
		points[6] = p7;
		points[7] = p8;
	}

	Vector getInterpolatedPoint(float t) {
		Vector point;
		for (int i = 0; i <= 8; ++i) {
			point = point + points[i % 8] * B(i, t);
		}
		return point;
	}
};

class BodyGeometry {
	Spline* catmullRoms[100];
public:
	BodyGeometry() {
		for (int i = 0; i < 100; ++i) {
			catmullRoms[i] = new Spline();
		}
		Bezier b0(Vector(-4.0, 1.8, 0.0), Vector(-4.0, 1.8, -0.5), Vector(-4.0, 3.5, -0.7), Vector(-4.0, 2.8, -0.7), Vector(-4.0, 4.0, 0.0), Vector(-4.0, 2.8, 0.7), Vector(-4.0, 3.5, 0.7), Vector(-4.0, 1.8, 0.5));
		for (int i = 0; i < 100; ++i) {
			catmullRoms[i]->addPoint(b0.getInterpolatedPoint(i / 100.0), 0.0);
		}
		Bezier b1(Vector(-3.0, -0.8, 0.0), Vector(-3.0, -0.8, -0.7), Vector(-3.0, 0.0, -1.0), Vector(-3.0, 1.5, -0.8), Vector(-3.0, 3.0, 0.0), Vector(-3.0, 1.5, 0.8), Vector(-3.0, 0.0, 1.0), Vector(-3.0, -0.8, 0.7));
		for (int i = 0; i < 100; ++i) {
			catmullRoms[i]->addPoint(b1.getInterpolatedPoint(i / 100.0), 0.2);
		}
		Bezier b2(Vector(-2.0, -1.5, 0.0), Vector(-2.0, -1.6, -1.5), Vector(-2.0, 0.0, -2.0), Vector(-2.0, 1.5, -1.5), Vector(-2.0, 2.5, 0.0), Vector(-2.0, 1.5, 1.5), Vector(-2.0, 0.0, 2.0), Vector(-2.0, -1.6, 1.5));
		for (int i = 0; i < 100; ++i) {
			catmullRoms[i]->addPoint(b2.getInterpolatedPoint(i / 100.0), 0.3);
		}
		Bezier b3(Vector(-1.0, -1.8, 0.0), Vector(-1.0, -2.0, -1.5), Vector(-1.0, 0.0, -2.0), Vector(-1.0, 1.5, -1.5), Vector(-1.0, 2.5, 0.0), Vector(-1.0, 1.5, 1.5), Vector(-1.0, 0.0, 2.0), Vector(-1.0, -2.0, 1.5));
		for (int i = 0; i < 100; ++i) {
			catmullRoms[i]->addPoint(b3.getInterpolatedPoint(i / 100.0), 0.4);
		}
		Bezier b4(Vector(0.0, -1.8, 0.0), Vector(0.0, -2.0, -1.5), Vector(0.0, 0.0, -2.0), Vector(0.0, 1.5, -1.5), Vector(0.0, 2.5, 0.0), Vector(0.0, 1.5, 1.5), Vector(0.0, 0.0, 2.0), Vector(0.0, -2.0, 1.5));
		for (int i = 0; i < 100; ++i) {
			catmullRoms[i]->addPoint(b4.getInterpolatedPoint(i / 100.0), 0.6);
		}
		Bezier b5(Vector(1.5, -1.8, 0.0), Vector(1.5, -2.0, -1.5), Vector(1.5, 0.0, -2.0), Vector(1.5, 1.5, -1.5), Vector(1.5, 2.5, 0.0), Vector(1.5, 1.5, 1.5), Vector(1.5, 0.0, 2.0), Vector(1.5, -2.0, 1.5));
		for (int i = 0; i < 100; ++i) {
			catmullRoms[i]->addPoint(b5.getInterpolatedPoint(i / 100.0), 0.8);
		}
		Bezier b6(Vector(3.0, 1.2, 0.0), Vector(3.0, 1.2, -0.01), Vector(3.0, 1.8, -0.02), Vector(3.0, 1.3, -0.02), Vector(3.0, 2.0, 0.0), Vector(3.0, 1.3, 0.02), Vector(3.0, 1.8, 0.02), Vector(3.0, 1.2, 0.01));
		for (int i = 0; i < 100; ++i) {
			catmullRoms[i]->addPoint(b6.getInterpolatedPoint(i / 100.0), 1.0);
		}
	}

	Spline* getCatmullRom(int n) {
		int norm = n % 100;
		return catmullRoms[norm];
	}

	~BodyGeometry() {
		for (int i = 0; i < 100; ++i) {
			delete catmullRoms[i];
		}
	}
};

class Sphere : public Object {
	Vector center;
	float radius;
	Material mat;
	float xAxis;
	float yAxis;
	float zAxis;
public:
	Sphere(Vector c, float r, Material m) : Object(true), center(c), radius(r), mat(m) {

	}

	void set() {
		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
	}

	void draw() {
		set();
		mat.setOGL();
		for (float fi = 0.0; fi <= 2 * PI + 0.1; fi = fi + 0.1) {
			glBegin(GL_TRIANGLE_STRIP);
			vertexOGL(0.0, 0.0);
			for (float tetha = 0.0; tetha <= PI + 0.1; tetha = tetha + 0.1) {
				vertexOGL(tetha, fi);
				vertexOGL(tetha, fi + 0.1);
			}
			vertexOGL(PI, 0.0);
			glEnd();
		}
		glPopMatrix();
	}

	void vertexOGL(float tetha, float fi) {
		glNormal3f(radius*sin(tetha)*cos(fi), radius*sin(tetha)*sin(fi), radius*cos(tetha));
		glVertex3f(radius*sin(tetha)*cos(fi), radius*sin(tetha)*sin(fi), radius*cos(tetha));
	}

	void setRotation(float x, float y, float z) {
		xAxis = x;
		yAxis = y;
		zAxis = z;
	}

	void setPosition(Vector p) {
		center = p;
	}

	float getDistance(Vector c) {
		return (center - c).Length();
	}

	void animate(float time) {

	}
};

class Cylinder : public Object {
	Vector base;
	float radius;
	float height;
	Material mat;
	float xAxis;
	float yAxis;
	float zAxis;
public:
	Cylinder(Vector b, float r, float h, Material m) : Object(true), base(b), radius(r), height(h), mat(m) {

	}

	void set() {
		glPushMatrix();
		glTranslatef(base.x, base.y, base.z);
		glRotatef(xAxis, 1.0, 0.0, 0.0);
		glRotatef(yAxis, 0.0, 1.0, 0.0);
		glRotatef(zAxis, 0.0, 0.0, 1.0);
	}

	void draw() {
		set();
		mat.setOGL();
		glBegin(GL_QUADS);
		for (float h = 0; h <= height / 2; h = h + 0.1) {
			for (float fi = 0; fi <= 2*PI + 0.1; fi = fi + 0.1) {
				vertexOGL(h, fi);
				vertexOGL(h + 0.1, fi);
				vertexOGL(h + 0.1, fi + 0.1);
				vertexOGL(h, fi + 0.1);
			}
		}
		for (float h = 0; h <= height / 2; h = h + 0.1) {
			for (float fi = 0; fi <= 2*PI + 0.1; fi = fi + 0.1) {
				vertexOGL((-1)*h, fi);
				vertexOGL((-1)*h - 0.1, fi);
				vertexOGL((-1)*h - 0.1, fi + 0.1);
				vertexOGL((-1)*h, fi + 0.1);
			}
		}
		glEnd();
		glPopMatrix();
	}

	void vertexOGL(float h, float fi) {
		glNormal3f(radius*cos(fi), 0.0, radius*sin(fi));
		glVertex3f(radius*cos(fi), h, radius*sin(fi));
	}

	void setRotation(float x, float y, float z) {
		xAxis = x;
		yAxis = y;
		zAxis = z;
	}

	void setPosition(Vector p) {
		base = p;
	}

	float getDistance(Vector c) {
		return (base - c).Length();
	}

	void animate(float time) {

	}
};

class Plain : public Object {
	Vector corner;
	float width;
	float length;
	Material mat;
	float xAxis;
	float yAxis;
	float zAxis;
public:
	Plain(Vector c, float w, float l, Material m) : Object(false), corner(c), width(w), length(l), mat(m) {

	}

	void set() {
		glPushMatrix();
		glTranslatef(corner.x, corner.y, corner.z);
	}

	void draw() {
		set();
		mat.setOGL();
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBegin(GL_QUADS);
		for (float x = 0.0; x <= width; x = x + 10.0) {
			for (float z = 0.0; z <= length; z = z + 10.0) {
				vertexOGL(x, z);
				vertexOGL(x + 10.0, z);
				vertexOGL(x + 10.0, z + 10.0);
				vertexOGL(x, z + 10.0);
			}
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	void vertexOGL(float x, float z) {
		glTexCoord2f(z/length, x/width);
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(10.0*x, 0.0, 10.0*z);
	}

	void setRotation(float x, float y, float z) {
		xAxis = x;
		yAxis = y;
		zAxis = z;
	}

	float getDistance(Vector c) {
		return (corner - c).Length();
	}

	void animate(float time) {

	}
};

class Conic : public Object {
	Vector base;
	float height;
	float radius;
	Material mat;
	float xAxis;
	float yAxis;
	float zAxis;
public:
	Conic(Vector b, float h, float r, Material m) : Object(true), base(b), height(h), radius(r), mat(m), xAxis(0.0), yAxis(0.0), zAxis(0.0) {

	}

	void set() {
		glPushMatrix();
		glTranslatef(base.x, base.y, base.z);
		glRotatef(xAxis, 1.0, 0.0, 0.0);
		glRotatef(yAxis, 0.0, 1.0, 0.0);
		glRotatef(zAxis, 0.0, 0.0, 1.0);
	}

	void draw() {
		set();
		mat.setOGL();
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, height, 0.0);
		for (float fi = 0.0; fi <= 2*PI + 0.1; fi = fi + 0.1) {
			vertexOGL(fi);
		}
		glEnd();
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0, 0.0, 0.0);
		for (float fi = 0.0; fi <= 2*PI + 0.1; fi = fi + 0.1) {
			vertexOGL(fi);
		}
		glEnd();
		glPopMatrix();
	}

	void vertexOGL(float fi) {
		glNormal3f(radius*cos(fi), radius*(radius / height), radius*sin(fi));
		glVertex3f(radius*cos(fi), 0.0, radius*sin(fi));
	}

	void setRotation(float x, float y, float z) {
		xAxis = x;
		yAxis = y;
		zAxis = z;
	}

	void setPosition(Vector p) {
		base = p;
	}

	float getDistance(Vector c) {
		return (base - c).Length();
	}

	void animate(float time) {

	}
};

class CsirguruHead : public Object {
	ObjectHolder objHolder;
	Vector center;
	float xAxis;
	float yAxis;
	float zAxis;
	float xScale;
	float yScale;
	float zScale;
	Vector speed;
public:
	CsirguruHead(Vector c) : Object(true), center(c), xAxis(0.0), yAxis(0.0), zAxis(0.0), xScale(1.0), yScale(1.0), zScale(1.0), speed(0.0, 0.0, 0.0) {
		Object* obj;
		Material white(Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), 50.0, 1.0, 1.0);
		Material black(Color(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), Color(0.0, 0.0, 0.0), 50.0, 1.0, 1.0);
		Material red(Color(1.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), Color(1.0, 0.0, 0.0), 50.0, 1.0, 1.0);
		Material orange(Color(1.0, 0.451, 0.0), Color(1.0, 1.0, 1.0), Color(1.0, 0.451, 0.0), 50.0, 1.0, 1.0);
		obj = new Sphere(Vector(-0.8, 1.5, 0.0), 1.0, white);
		objHolder.addObject(obj);
		obj = new Sphere(Vector(0.8, 1.5, 0.0), 1.0, white);
		objHolder.addObject(obj);

		obj = new Sphere(Vector(-0.8, 1.5, 0.7), 0.5, black);
		objHolder.addObject(obj);
		obj = new Sphere(Vector(0.8, 1.5, 0.7), 0.5, black);
		objHolder.addObject(obj);

		obj = new Conic(Vector(0.0, 1.5, 0.0), 4.0, 0.5, red);
		objHolder.addObject(obj);
		obj = new Conic(Vector(0.0, 1.5, 0.0), 4.0, 0.5, red);
		obj->setRotation(-30, 0.0, 0.0);
		objHolder.addObject(obj);
		obj = new Conic(Vector(0.0, 1.5, 0.0), 4.0, 0.5, red);
		obj->setRotation(-60.0, 0.0, 0.0);
		objHolder.addObject(obj);
		obj = new Conic(Vector(0.0, 1.5, 0.0), 4.0, 0.5, red);
		obj->setRotation(-90.0, 0.0, 0.0);
		objHolder.addObject(obj);
		obj = new Conic(Vector(0.0, 1.5, 0.0), 2.5, 0.5, orange);
		obj->setRotation(105.0, 0.0, 0.0);
		objHolder.addObject(obj);
	}

	void set() {
		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		glRotatef(xAxis, 1.0, 0.0, 0.0);
		glRotatef(yAxis, 0.0, 1.0, 0.0);
		glRotatef(zAxis, 0.0, 0.0, 1.0);
		glScalef(xScale, yScale, zScale);
	}

	void draw() {
		set();
		for (int i = 0; i < objHolder.getObjNum(); ++i) {
			objHolder.getObject(i)->draw();
		}
		glPopMatrix();
	}

	void setRotation(float x, float y, float z) {
		xAxis = x;
		yAxis = y;
		zAxis = z;
	}

	void setScale(float x, float y, float z) {
		xScale = x;
		yScale = y;
		zScale = z;
	}

	bool getDead() {
		if (center.y < -5.0) {
			return true;
		}
		return false;
	}

	void blast(Vector c, Vector csirguruCenter) {
		Vector s = ((csirguruCenter + center) - c).normalize()*7.0;
		speed = s;
	}

	void setPosition(Vector p) {
		center = p;
	}

	float getDistance(Vector c) {
		return (center - c).Length();
	}

	void animate(float time) {
		if (-7.0 < center.y) {
			speed = speed + gravity*(time / 1000.0);
		}
		else {
			speed = Vector(0.0, 0.0, 0.0);
		}
		xAxis += time;
		setPosition(center + speed*(time / 1000.0));
	}

	~CsirguruHead() {
		for (int i = 0; i < objHolder.getObjNum(); ++i) {
			delete objHolder.getObject(i);
		}
	}
};

class CsirguruBody : public Object {
	Vector center;
	float xAxis;
	float yAxis;
	float zAxis;
	float xScale;
	float yScale;
	float zScale;
	BodyGeometry body;
	Material mat;
	Vector speed;
public:
	CsirguruBody(Vector c) : Object(true), center(c), xAxis(0.0), yAxis(0.0), zAxis(0.0), xScale(1.0), yScale(1.0), zScale(1.0), mat(Color(0.412, 0.184, 0.0), Color(1.0, 1.0, 1.0), Color(0.412, 0.184, 0.0), 50.0, 1.0, 1.0), speed(0.0, 0.0, 0.0) {
		
	}

	void set() {
		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		glRotatef(xAxis, 1.0, 0.0, 0.0);
		glRotatef(yAxis, 0.0, 1.0, 0.0);
		glRotatef(zAxis, 0.0, 0.0, 1.0);
		glScalef(xScale, yScale, zScale);
	}

	void draw() {
		set();
		mat.setOGL();
		glBegin(GL_QUADS);
		for (int r = 0; r <= 100; ++r) {
			for (float t = 0.0; t < 0.98; t = t + 0.01) {
				vertexOGL(r, t);
			}
		}
		glEnd();
		glPopMatrix();
	}

	void vertexOGL(int r, float t) {
		Vector v0 = body.getCatmullRom(r)->getSurfacePoint(t);
		Vector v1 = body.getCatmullRom(r + 1)->getSurfacePoint(t);
		Vector v2 = body.getCatmullRom(r + 1)->getSurfacePoint(t + 0.01);
		Vector v3 = body.getCatmullRom(r)->getSurfacePoint(t + 0.01);
		Vector n0 = (v1 - v0) % (v3 - v0);
		Vector n1 = (v2 - v1) % (v3 - v1);
		Vector n2 = (v0 - v2) % (v1 - v2);
		Vector n3 = (v0 - v3) % (v1 - v3);
		glNormal3f(n0.x, n0.y, n0.z);
		glVertex3f(v0.x, v0.y, v0.z);
		glNormal3f(n1.x, n1.y, n1.z);
		glVertex3f(v1.x, v1.y, v1.z);
		glNormal3f(n2.x, n2.y, n2.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glNormal3f(n3.x, n3.y, n3.z);
		glVertex3f(v3.x, v3.y, v3.z);
	}

	void setRotation(float x, float y, float z) {
		xAxis = x;
		yAxis = y;
		zAxis = z;
	}

	void setScale(float x, float y, float z) {
		xScale = x;
		yScale = y;
		zScale = z;
	}

	bool getDead() {
		if (center.y < -5.0) {
			return true;
		}
		return false;
	}

	void blast(Vector c, Vector csirguruCenter) {
		Vector s = ((csirguruCenter + center) - c).normalize()*7.0;
		speed = s;
	}

	void setPosition(Vector p) {
		center = p;
	}

	float getDistance(Vector c) {
		return (center - c).Length();
	}

	void animate(float time) {
		if (-7.0 < center.y) {
			speed = speed + gravity*(time / 1000.0);
		}
		else {
			speed = Vector(0.0, 0.0, 0.0);
		}
		zAxis += time;
		setPosition(center + speed*(time / 1000.0));
	}
};

class CsirguruLeg : public Object {
	ObjectHolder objHolder;
	Vector kneeJoint;
	float xAxis;
	float yAxis;
	float zAxis;
	float xScale;
	float yScale;
	float zScale;
	float kneeRotation;
	float ankleRotation;
	Vector speed;
public:
	CsirguruLeg(Vector k) : Object(true), kneeJoint(k), xAxis(0.0), yAxis(0.0), zAxis(0.0), kneeRotation(20.0), ankleRotation(40.0), xScale(1.0), yScale(1.0), zScale(1.0), speed(0.0, 0.0, 0.0) {
		float kneeAlpha = kneeRotation*PI / 180;
		float ankleAlpha = (ankleRotation / 2.0)*PI / 180;
		Object* obj;
		Material ma(Color(0.659, 0.647, 0.239), Color(1.0, 1.0, 1.0), Color(0.659, 0.647, 0.239), 50.0, 1.0, 1.0);
		float upperHDif = cos(kneeAlpha)*1.0;
		float upperWDif = sin(kneeAlpha)*1.0;
		float lowerHDif = cos(ankleAlpha)*1.0;
		float lowerWDif = sin(ankleAlpha)*1.0;
		obj = new Sphere(Vector(0.0, 0.0, 0.0), 0.2, ma);
		objHolder.addObject(obj);
		obj = new Cylinder(Vector(upperWDif, (-1.0)*upperHDif, 0.0), 0.2, 2.0, ma);
		obj->setRotation(0.0, 0.0, kneeRotation);
		objHolder.addObject(obj);
		obj = new Sphere(Vector(2.0*upperWDif, (-2.0)*upperHDif, 0.0), 0.2, ma);
		objHolder.addObject(obj);
		obj = new Cylinder(Vector(2.0*upperWDif - lowerWDif, (-2.0)*upperHDif - lowerHDif, 0.0), 0.2, 2.0, ma);
		obj->setRotation(0.0, 0.0, (-1.0)*ankleRotation / 2.0);
		objHolder.addObject(obj);
		obj = new Sphere(Vector(2.0*upperWDif - 2.0*lowerWDif, (-2.0)*upperHDif-2.0*lowerHDif, 0.0), 0.2, ma);
		objHolder.addObject(obj);
		obj = new Conic(Vector(2.0*upperWDif - 2.0*lowerWDif, (-2.0)*upperHDif - 2.0*lowerHDif, 0.0), 1.5, 0.2, ma);
		obj->setRotation(0.0, 0.0, 97.6);
		objHolder.addObject(obj);
	}

	void set() {
		glPushMatrix();
		glTranslatef(kneeJoint.x, kneeJoint.y, kneeJoint.z);
		glRotatef(xAxis, 1.0, 0.0, 0.0);
		glRotatef(yAxis, 0.0, 1.0, 0.0);
		glRotatef(zAxis, 0.0, 0.0, 1.0);
		glScalef(xScale, yScale, zScale);

		float kneeAlpha = kneeRotation*PI / 180;
		float ankleAlpha = (ankleRotation / 2.0)*PI / 180;
		float upperHDif = cos(kneeAlpha)*1.0;
		float upperWDif = sin(kneeAlpha)*1.0;
		float lowerHDif = cos(ankleAlpha)*1.0;
		float lowerWDif = sin(ankleAlpha)*1.0;
		Object* obj;
		obj = objHolder.getObject(1);
		obj->setPosition(Vector(upperWDif, (-1.0)*upperHDif, 0.0));
		obj->setRotation(0.0, 0.0, kneeRotation);
		obj = objHolder.getObject(2);
		obj->setPosition(Vector(2.0*upperWDif, (-2.0)*upperHDif, 0.0));
		obj = objHolder.getObject(3);
		obj->setPosition(Vector(2.0*upperWDif - lowerWDif, (-2.0)*upperHDif - lowerHDif, 0.0));
		obj->setRotation(0.0, 0.0, (-1.0)*ankleRotation / 2.0);
		obj = objHolder.getObject(4);
		obj->setPosition(Vector(2.0*upperWDif - 2.0*lowerWDif, (-2.0)*upperHDif - 2.0*lowerHDif, 0.0));
		obj = objHolder.getObject(5);
		obj->setPosition(Vector(2.0*upperWDif - 2.0*lowerWDif, (-2.0)*upperHDif - 2.0*lowerHDif, 0.0));
	}

	void draw() {
		set();
		for (int i = 0; i < objHolder.getObjNum(); ++i) {
			objHolder.getObject(i)->draw();
		}
		glPopMatrix();
	}

	void setRotation(float x, float y, float z) {
		xAxis = x;
		yAxis = y;
		zAxis = z;
	}

	void setJoints(float knee, float ankle) {
		kneeRotation = knee;
		ankleRotation = ankle;
	}

	void inverseKinematics(float dx, float dy) {
		float kneeAlpha = kneeRotation*PI / 180.0;
		float ankleAlpha = ankleRotation*PI / 180.0;
		float detA = (1.0*cos(kneeAlpha + ankleAlpha))*((-1.0)*1.0*sin(kneeAlpha) - 1.0*sin(kneeAlpha + ankleAlpha)) - ((-1.0)*sin(kneeAlpha + ankleAlpha))*(1.0*cos(kneeAlpha) + 1.0*cos(kneeAlpha + ankleAlpha));
		float dAlpha = (dx*1.0*cos(kneeAlpha + ankleAlpha) + dy*1.0*sin(kneeAlpha + ankleAlpha)) / detA;
		float dBeta = ((dy*((-1.0)*1.0*sin(kneeAlpha) - 1.0*sin(kneeAlpha + ankleAlpha))) - (dx*(1.0*cos(kneeAlpha) + 1.0*cos(kneeAlpha + ankleAlpha)))) / detA;
		setJoints(kneeRotation + (dAlpha*180.0 / PI), ankleRotation + (dBeta*180.0 / PI));
	}

	void setScale(float x, float y, float z) {
		xScale = x;
		yScale = y;
		zScale = z;
	}

	float getHeight() {
		float kneeAlpha = kneeRotation*PI / 180;
		float ankleAlpha = (ankleRotation / 2.0)*PI / 180;
		return cos(kneeAlpha)*1.0 + cos(ankleAlpha)*1.0 + 0.1;
	}

	bool getDead() {
		if (kneeJoint.y < -5.0) {
			return true;
		}
		return false;
	}

	void blast(Vector c, Vector csirguruCenter) {
		Vector s = ((csirguruCenter + kneeJoint) - c).normalize()*7.0;
		speed = s;
	}

	void setPosition(Vector p) {
		kneeJoint = p;
	}

	float getDistance(Vector c) {
		return (kneeJoint - c).Length();
	}

	void animate(float time) {
		if (-7.0 < kneeJoint.y) {
			speed = speed + gravity*(time / 1000.0);
		}
		else {
			speed = Vector(0.0, 0.0, 0.0);
		}
		yAxis += time;
		setPosition(kneeJoint + speed*(time / 1000.0));
	}

	~CsirguruLeg() {
		for (int i = 0; i < objHolder.getObjNum(); ++i) {
			delete objHolder.getObject(i);
		}
	}
};

enum JumpState {
	preJump, jump, postJump
};

class Csirguru : public Object {
	ObjectHolder objHolder;
	Vector kneeJoint;
	float xAxis;
	float yAxis;
	float zAxis;
	float xScale;
	float yScale;
	float zScale;
	Vector speed;
	float prevJump;
	float turn;
	bool jumped;
	JumpState state;
	float preJumpTimer;
	float forwardTim;
	float postJumpTimer;
	bool alive;
public:
	Csirguru(Vector k) : Object(true), kneeJoint(k), xAxis(0.0), yAxis(0.0), zAxis(0.0), xScale(1.0), yScale(1.0), zScale(1.0), speed(0.0, 0.0, 0.0), prevJump(0.0), turn(0.0), jumped(false), state(preJump), preJumpTimer(0.0), postJumpTimer(0.0), forwardTim(0.0), alive(true) {
		Object* obj;
		obj = new CsirguruHead(Vector(-2.2, 1.5, 0.0));
		obj->setRotation(0.0, -90.0, 0.0);
		obj->setScale(0.4, 0.4, 0.4);
		objHolder.addObject(obj);
		obj = new CsirguruBody(Vector(0.0, 0.9, 0.0));
		obj->setScale(0.5, 0.5, 0.5);
		objHolder.addObject(obj);
		obj = new CsirguruLeg(Vector(0.0, 0.0, 0.0));
		obj->setScale(0.5, 0.5, 0.5);
		objHolder.addObject(obj);
	}

	void set() {
		glPushMatrix();
		glTranslatef(kneeJoint.x, kneeJoint.y, kneeJoint.z);
		glRotatef(xAxis, 1.0, 0.0, 0.0);
		glRotatef(yAxis, 0.0, 1.0, 0.0);
		glRotatef(zAxis, 0.0, 0.0, 1.0);
		glScalef(xScale, yScale, zScale);
	}

	void draw() {
		set();
		for (int i = 0; i < objHolder.getObjNum(); ++i) {
			objHolder.getObject(i)->draw();
		}
		glPopMatrix();
	}

	void setRotation(float x, float y, float z) {
		xAxis = x;
		yAxis = y;
		zAxis = z;
	}

	void setScale(float x, float y, float z) {
		xScale = x;
		yScale = y;
		zScale = z;
	}

	void setSpeed(Vector s) {
		speed = s;
	}

	void setPosition(Vector v) {
		kneeJoint = v;
	}

	float getDistance(Vector c) {
		return (kneeJoint - c).Length();
	}

	bool getDead() {
		if (((CsirguruHead*)objHolder.getObject(0))->getDead() && ((CsirguruBody*)objHolder.getObject(1))->getDead() && ((CsirguruLeg*)objHolder.getObject(2))->getDead()) {
			return true;
		}
		return false;
	}

	void blast(Vector c) {
		((CsirguruHead*)objHolder.getObject(0))->blast(c, kneeJoint);
		((CsirguruBody*)objHolder.getObject(1))->blast(c, kneeJoint);
		((CsirguruLeg*)objHolder.getObject(2))->blast(c, kneeJoint);
		alive = false;
	}

	void animate(float time) {
		if (!alive) {
			setHasshadow(false);
			for (int i = 0; i < objHolder.getObjNum(); ++i) {
				objHolder.getObject(i)->animate(time);
			}
			return;
		}
		if (objHolder.getObject(objHolder.getObjNum() - 1)->getHeight() * yScale < kneeJoint.y) {
			setSpeed(speed + gravity*(time/1000.0));
			if (state == postJump && speed.y < 0.0) {
				float mov;
				for (float t = 0.0; t <= time; t++) {
					if (forwardTim < 150) {
						mov = -0.002;
						objHolder.getObject(objHolder.getObjNum() - 1)->inverseKinematics(mov, mov);
						forwardTim++;
					}
				}
			}
		}
		else {
			if (speed.y < 0.0) {
				setSpeed(Vector(0.0, 0.0, 0.0));
			}
			kneeJoint.y = objHolder.getObject(objHolder.getObjNum() - 1)->getHeight() * yScale;
		}
		
		if (speed.Length() < 0.1 && (turn < 1.5 && (-1.5) < turn)) {
			turn = 0.0;
			if (jumped) {
				if (rand() % 2) {
					turn = 30.0;
				}
				else {
					turn = -30.0;
				}
				jumped = false;
			}
			else {
				if (1000.0 < prevJump) {
					switch (state) {
					case preJump:
					{
						for (float t = 0.0; t <= time; ++t) {
							float mov;
							if (preJumpTimer < 300.0) {
								if (preJumpTimer < 250.0) {
									if (preJumpTimer < 125.0) {
										mov = -0.004;
									}
									else {
										mov = 0.004;
									}
									objHolder.getObject(objHolder.getObjNum() - 1)->inverseKinematics(mov, mov);

									float rotationAlpha = yAxis*PI / 180.0;
									setPosition(kneeJoint + Vector(mov*(-1.0)*cos(rotationAlpha), 0.0, mov * sin(rotationAlpha)));
									preJumpTimer++;
								}
								else {
									mov = -0.02;
									objHolder.getObject(objHolder.getObjNum() - 1)->inverseKinematics((-0.55)*mov, mov);
									preJumpTimer++;
								}
							}
							else {
								state = jump;
								preJumpTimer = 0.0;
								break;
							}
						}
						break;
					}
					case jump:
					{
						float rotationAlpha = yAxis*PI / 180.0;
						setSpeed(Vector((-2.0)*cos(rotationAlpha) / sqrt(2), 2 / sqrt(2), 2 * sin(rotationAlpha) / sqrt(2)));
						state = postJump;
						break;
					}
					case postJump:
					{	
						float mov;
						for (float t = 0.0; t <= time; t++) {
							if (postJumpTimer < 150) {
								mov = 0.002;
								objHolder.getObject(objHolder.getObjNum() - 1)->inverseKinematics(mov, mov);
								float rotationAlpha = yAxis*PI / 180.0;
								setPosition(kneeJoint + Vector(mov*(-1.0)*cos(rotationAlpha), 0.0, mov * sin(rotationAlpha)));
								postJumpTimer++;
							}
							if (postJumpTimer < 450.0) {
								if (postJumpTimer < 200.0) {
									mov = -0.02;
									objHolder.getObject(objHolder.getObjNum() - 1)->inverseKinematics(mov, -0.85*mov);
									postJumpTimer++;
								}
								else {
									((CsirguruLeg*)objHolder.getObject(objHolder.getObjNum() - 1))->setJoints(20.0, 40.0);
								}
								if (postJumpTimer < 450.0) {
									postJumpTimer++;
								}
								else {
									((CsirguruLeg*)objHolder.getObject(objHolder.getObjNum() - 1))->setJoints(20.0, 40.0);
								}
							}
							else {
								((CsirguruLeg*)objHolder.getObject(objHolder.getObjNum() - 1))->setJoints(20.0, 40.0);
								state = preJump;
								postJumpTimer = 0.0;
								forwardTim = 0.0;
								prevJump = 0.0;
								jumped = true;
							}
						}
						break;
						}
					}
				}
				else {
					((CsirguruLeg*)objHolder.getObject(objHolder.getObjNum() - 1))->setJoints(20.0, 40.0);
					prevJump += time;
				}
			}
		}
		else {
			if (turn <= -1.5) {
				turn += 0.03*time;
				yAxis -= 0.03*time;
			}
			if (1.5 <= turn) {
				turn -= 0.03*time;
				yAxis += 0.03*time;
			}
		}
		setPosition(kneeJoint + speed*(time / 1000.0));
	}

	~Csirguru() {
		for (int i = 0; i < objHolder.getObjNum(); ++i) {
			delete objHolder.getObject(i);
		}
	}
};

class Bomb : public Object {
	Vector center;
	float radius;
	ObjectHolder objHolder;
	float xScale;
	float yScale;
	float zScale;
	Vector speed;
	bool dropped;
	float originalHeight;
public:
	Bomb(Vector c) : Object(true), center(c), speed(0.0, 0.0, 0.0), dropped(false), originalHeight(c.y) {
		Object* obj;
		Material red(Color(1.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), Color(1.0, 0.0, 0.0), 50.0, 1.0, 1.0);
		obj = new Sphere(Vector(0.0, 0.0, 0.0), 1.0, red);
		objHolder.addObject(obj);
		Material gray(Color(0.7, 0.7, 0.7), Color(1.0, 1.0, 1.0), Color(0.7, 0.7, 0.7), 30.0, 1.0, 1.0);
		obj = new Conic(Vector(0.0, 0.0, 0.0), 3.0, 0.9, gray);
		objHolder.addObject(obj);
		obj = new Cylinder(Vector(0.0, 3.0, 0.0), 1.0, 0.8, gray);
		objHolder.addObject(obj);
	}

	void set() {
		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		glScalef(0.3, 0.3, 0.3);
	}

	void draw() {
		set();
		for (int i = 0; i < objHolder.getObjNum(); ++i) {
			objHolder.getObject(i)->draw();
		}
		glPopMatrix();
	}

	void setRotation(float x, float y, float z){

	}

	void setPosition(Vector v) {
		if (!dropped) {
			center = v;
		}
	}

	Vector getPosition() {
		return center;
	}

	float getDistance(Vector c) {
		return (center - c).Length();
	}

	void drop() {
		dropped = true;
	}

	void animate(float time) {
		if (0.3 < center.y && dropped) {
			speed = speed + gravity*(time / 1000.0);
		}
		if (center.y <= 0.3) {
			speed = Vector(0.0, 0.0, 0.0);
			setPosition(Vector(center.x, originalHeight, center.z));
			dropped = false;
		}
		center = center + speed*(time / 1000.0);
	}

	~Bomb() {
		for (int i = 0; i < objHolder.getObjNum(); ++i) {
			delete objHolder.getObject(i);
		}
	}
};

class Camera {
	Vector eye;
	Vector lookAt;
	Vector vUp;
	float fov;
	float nearP;
	float farP;
public:
	Camera(Vector e, Vector l, Vector v, float f, float np, float fp) : eye(e), lookAt(l), vUp(v), fov(f), nearP(np), farP(fp) {

	}

	void setOGL() {
		glViewport(0, 0, screenWidth, screenHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fov, (float)screenWidth / (float)screenHeight, nearP, farP);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, vUp.x, vUp.y, vUp.z);
	}
};

class Light {
	Vector posv;
	Color Ids;
	Color Ias;
	Color Iss;

public:
	Light(Vector p, Color a, Color d, Color s) : posv(p), Ias(a), Ids(d), Iss(s) {

	}

	void setOGL() {
		float pos[4] = { posv.x, posv.y, posv.z, 0 };
		float Ia[4] = { Ias.r, Ias.g, Ias.b, 1 }, Id[4] = { Ids.r, Ids.g, Ids.b, 1 }, Is[4] = { Iss.r, Iss.g, Iss.b, 1 };

		glLightfv(GL_LIGHT0, GL_AMBIENT, Ia);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, Id);
		glLightfv(GL_LIGHT0, GL_SPECULAR, Is);
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		glEnable(GL_LIGHT0);
	}

	Vector getPosition() {
		return posv;
	}
};

class Scene {
	Camera camera;
	Light sun;
	ObjectHolder objHolder;
	Bomb* bomb;
public:
	Scene() : camera(Camera(Vector(0.0, 3.0, 0.0), Vector(0.0, 3.0, -1.0), Vector(0.0, 1.0, 0.0), 54, 1, 100)), sun(Light(Vector(1.0, 1.0, 1.0), Color(0.0, 0.0, 0.0), Color(0.3, 0.3, 0.3), Color(2.0, 2.0, 2.0))) {

	}

	void build() {
		Object* obj;
		bomb = new Bomb(Vector(0.0, 3.0, -10.0));
		Material whiteFloor(Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0), 50, 1.0, 1.0);
		obj = new Plain(Vector(-100.0, 0.0, -100.0), 200, 100, whiteFloor);
		objHolder.addObject(obj);
	}



	void render() {
		camera.setOGL();
		sun.setOGL();
		glEnable(GL_NORMALIZE);

		glEnable(GL_LIGHTING);
		draw(false);

		Vector lightdir = sun.getPosition();

		float shadow_mtx[4][4] = { 1,                         0,       0,                       0,
								-lightdir.x / lightdir.y,	  0,     -lightdir.z / lightdir.y,  0,
									0,                        0,      1,                        0,
									0,                    0.001,      0,                        1 };

		glMultMatrixf(&shadow_mtx[0][0]);

		glDisable(GL_LIGHTING);
		glColor3f(0, 0, 0);
		draw(true);

	}

	void draw(bool shade) {
		if (shade) {
			for (int i = 0; i < objHolder.getObjNum(); ++i) {
				Object* obj = objHolder.getObject(i);
				if (obj->shadow()) {
					obj->draw();
				}
			}
			bomb->draw();
		}
		else {
			for (int i = 0; i < objHolder.getObjNum(); ++i) {
				objHolder.getObject(i)->draw();
			}
			bomb->draw();
		}
	}

	void animate(float time) {
		for (int i = 1; i < objHolder.getObjNum(); ++i) {
			Object* o = objHolder.getObject(i);
			if (((Csirguru*)o)->getDead()) {
				delete o;
				objHolder.deleteObjAt(i);
			}
		}
		for (int i = 0; i < objHolder.getObjNum(); ++i) {
			objHolder.getObject(i)->animate(time);
		}
		bomb->animate(time);
		if (bomb->getPosition().y <= 0.3) {
			explosion(Vector(bomb->getPosition()));
		}
	}

	void dropBomb() {
		bomb->drop();
	}

	void moveBomb(Vector d) {
		bomb->setPosition(bomb->getPosition() + d);
	}

	void explosion(Vector c) {
		for (int i = 1; i < objHolder.getObjNum(); ++i) {
			Object* o = objHolder.getObject(i);
			if (o->getDistance(c) < 2.0) {
				objHolder.getObject(i)->blast(c);
			}
		}
	}

	void spawnCsirguru() {
		Vector p = bomb->getPosition();
		Object* obj = new Csirguru(Vector(p.x, 1.0, p.z));
		obj->setScale(0.5, 0.5, 0.5);
		float deg = rand() % 360;
		obj->setRotation(0.0, deg, 0.0);
		objHolder.addObject(obj);
	}

	~Scene() {
		for (int i = 0; i < objHolder.getObjNum(); ++i) {
			delete objHolder.getObject(i);
		}
		delete bomb;
	}
};

Scene scene;
long lastFrame;
unsigned char image[512 * 512 * 3];
long lastSpawn;

void onInitialization( ) { 
	

	for (int i = 0; i < 512; ++i) {
		for (int j = 0; j < 512; ++j) {
				if (i % 2 <= 0) {
					image[3 * i * 512 + j * 3 + 0] = 0;
					image[3 * i * 512 + j * 3 + 1] = 140;
					image[3 * i * 512 + j * 3 + 2] = 0;
				}
				else {
					image[3 * i * 512 + j * 3 + 0] = 66;
					image[3 * i * 512 + j * 3 + 1] = 38;
					image[3 * i * 512 + j * 3 + 2] = 0;
				}
		}
	}

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_TEXTURE_2D);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, &image[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	scene.build();
	lastFrame = glutGet(GLUT_ELAPSED_TIME);
	lastSpawn = 0;
}

void onDisplay( ) {
	glClearColor(0.521, 0.878, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.render();

	glutSwapBuffers();
}

bool enabled = true;

void onKeyboard(unsigned char key, int x, int y) {
	if (key == 'd') {
		scene.moveBomb(Vector(1.0, 0.0, 0.0));
	}
	if (key == 'w') {
		scene.moveBomb(Vector(0.0, 0.0, -1.0));
	}
	if (key == 'a') {
		scene.moveBomb(Vector(-1.0, 0.0, 0.0));
	}
	if (key == 'y') {
		scene.moveBomb(Vector(0.0, 0.0, 1.0));
	}

	/*if (key == 'p') {
		enabled = true;
	}*/

	if (key == ' ') {
		scene.dropBomb();
	}
}

void onKeyboardUp(unsigned char key, int x, int y) {

}


void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		glutPostRedisplay( );
}


void onMouseMotion(int x, int y)
{

}



void onIdle( ) {
	if (!enabled) {
		return;
	}
	if (3000 <= lastSpawn) {
		scene.spawnCsirguru();
		lastSpawn = 0;
	}
	 float t = glutGet(GLUT_ELAPSED_TIME) - lastFrame;
	 lastSpawn += t;
	 if (50.0 < t) {
		 float remaining = 0;
		 for (float tim = 50.0; tim < t; tim = tim + 50.0) {
			 scene.animate(50);
			 remaining = t - tim;
		 }
		 scene.animate(remaining);
	 }
	 else {
		 scene.animate(t);
	 }
	 lastFrame = glutGet(GLUT_ELAPSED_TIME);
	 glutPostRedisplay();
}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
	{glutInit(&argc, argv); 				// GLUT inicializalasa
	glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel 
	glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

	glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

	glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
	glLoadIdentity();

	onInitialization();					// Az altalad irt inicializalast lefuttatjuk

	glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
	glutMouseFunc(onMouse);
	glutIdleFunc(onIdle);
	glutKeyboardFunc(onKeyboard);
	glutKeyboardUpFunc(onKeyboardUp);
	glutMotionFunc(onMouseMotion);

	glutMainLoop();
	}	// Esemenykezelo hurok
	_CrtDumpMemoryLeaks();
    return 0;
}

