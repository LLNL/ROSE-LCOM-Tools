package body points is

  function origin return Point2D is
  begin
    return Point2D'(0,0);
  end;

  function "+" (lhs, rhs : Point2D) return Point2D is
  begin
    return (lhs.x + rhs.x, lhs.y + rhs.y);
  end;

  function origin return Point3D is
  begin
    return Point3D'(0,0,0);
  end;

  function "+" (lhs, rhs : Point3D) return Point3D is
  begin
    -- alternative: return Point3D'(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    return Point3D'(Point2D(lhs) + Point2D(rhs) with lhs.z + rhs.z);
  end;

end points;
