package NestedAlias

type NestedAlias struct {
    i,j int
}

func (c NestedAlias) method() {
    c.i = 1
}

func (c NestedAlias) method2() {
    c.j = 2
    var aliasMethod = c.method
    aliasMethod()
    return
}