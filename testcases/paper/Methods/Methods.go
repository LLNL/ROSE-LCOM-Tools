package methods

import "fmt"

func outsideMethod() {
    fmt.Println("Outside method called.")
}

type Methods struct {}

func (m Methods) methodOne() {
    fmt.Println("Method one called.")
}

func (m Methods) methodTwo() {
    fmt.Println("Method two called.")
    m.methodOne() // Calls methodOne
}

func (m Methods) methodThree() {
    fmt.Println("Method three called.")
    outsideMethod() // Calls outside method
}
