package NestedClasses

type A struct {
    i, j, k int
    B struct {
        x, y, z int
    }
}

func (a A) GetDataI() int {
    return a.i
}

func (a A) GetDataJ() int {
    return a.j
}

func (b A) GetDataX() int {
    return b.B.x
}

func (b A) GetDataY() int {
    return b.B.y
}
