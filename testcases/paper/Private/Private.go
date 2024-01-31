package Private

type Private struct {
    i, j int
}

func (c Private) PublicMethod() {
    c.j = 2
}

func (c Private) privateMethod() {
    c.j = 2
}

// Private don't count: LCOM4 = 1 (outputs nothing)
// Private count:       LCOM4 = 2