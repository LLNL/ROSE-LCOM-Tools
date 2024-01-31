package Raising

import "fmt"

type Raising struct {
	NetworkFailure        error
	DiskFailure           error
	InternalStatusFailure error
}

func (c Raising) NetworkCheck() error {
	return c.NetworkFailure
}

func (c Raising) DiskCheck() error {
	return c.DiskFailure
}

func (c Raising) InternalStatusCheck() error {
	return c.InternalStatusFailure
}

func (c Raising) AllError() error {
	err := fmt.Errorf("%w; %w; %w",
					  c.NetworkFailure, c.DiskFailure, c.InternalStatusFailure)
	return err
}

// If they work, then LCOM4 should properly group all of these errors on AllError()
// Errors handled by tool:     LCOM4 = 1
// Errors not handled by tool: LCOM4 = 4