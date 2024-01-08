package points is

  type Point2D is tagged record
         x,y : integer := 0;
       end record;

  function origin return Point2D;
  function "+" (lhs, rhs : Point2D) return Point2D;

  type Point3D is new Point2D with record
         z : integer := 0;
       end record;

  function origin return Point3D;
  function "+" (lhs, rhs : Point3D) return Point3D;

end points;
