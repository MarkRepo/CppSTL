class Point2d {
 public:
  Point2d(float = 0.0, float = 0.0);
  virtual ~Point2d();
  virtual void mumble();
  virtual float z();

 protected:
  float _x, _y;
};

class Point3d : public virtual Point2d {
 public:
  Point3d(float = 0.0, float = 0.0, float = 0.0) ~Point3d();
  float z();
  float _z;
};

int main() {
  return sizeof(Point3d);
}
