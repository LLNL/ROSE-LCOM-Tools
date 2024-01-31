package Function

type Function struct {
    i,j int
}

func (c Function) method() {
    c.i = 1
}

func (c Function) method2() {
    c.j = 2
    var aliasMethod = c.method
    aliasMethod()
    return
}

// Function aliases don't count: LCOM4 = 2
// Functions aliases count:      LCOM4 = 1