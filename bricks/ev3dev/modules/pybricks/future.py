# MIT License
#
# Copyright (c) 2017 Laurens Valk
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

"""Experimental features to be rewritten in C code at a later stage."""

from .parameters import Stop
from math import pi


class Mechanism():
    """Class to control a motor with predefined target angles."""

    def __init__(self, motor, speed, targets, after_stop, reset_forward, reset_torque):
        """Initialize the mechanism settings.

        Arguments:
            motor {motor} -- Previously initialized motor object
            speed {int} -- Mechanism speed while moving to a target
            targets {dict} -- Dictionary of keys (e.g. strings, colors, or sensor values) with corresponding mechanism target angles
            after_stop {const} -- What to do after reaching a target: COAST, BRAKE, or Stop.hold
            reset_forward {bool} -- Go forward until hitting the endstop (True) or go backwards until hitting the endstop (False) (default: {True})
            reset_torque {int} -- Percentage of the maximum torque applied while resetting
        """

        self.motor = motor
        self.speed_abs = speed if speed > 0 else -speed
        self.targets = targets
        self.after_stop = after_stop
        self.reset_forward = reset_forward
        self.reset_torque = reset_torque
        if 'reset' not in self.targets.keys():
            # TODO: Raise error
            pass

    def reset(self):
        """Move towards the endstop and reset angle accordingly."""
        # Get speed with sign
        speed = self.speed_abs if self.reset_forward else -self.speed_abs

        # Temporarily set specified duty limit (TODO: First GET old setting so we can return it afterwards)
        self.motor.settings(self.reset_torque, 0, 500, 5, 1000, 1, 1000, 1000, 100, 800, 800, 5) #( TODO: Implement keyword args to change only the two relevant settings)
        self.motor.run_stalled(speed, Stop.hold)
        self.motor.reset_angle(self.targets['reset'])
        self.motor.settings(100, 2, 500, 5, 1000, 1, 1000, 1000, 100, 800, 800, 5) #( TODO: Implement keyword args to change only the two relevant settings)

        # Because reset_angle coasts the motor, ensure we stay on reset target with configured stop type
        self.go('reset')

    def go(self, target_key, wait=True):
        """Go to the target specified by the key."""
        # TODO: make speed and after_stop type optional as well, defaulting to initialized values
        self.motor.run_target(self.speed_abs, self.targets[target_key], self.after_stop, wait)

class DriveBase():
    def __init__(self, left_motor, right_motor, wheel_diameter, axle_track):
        self.left_motor = left_motor
        self.right_motor = right_motor
        self.wheel_diameter = wheel_diameter
        self.axle_track = axle_track

    def drive(self, speed, steering):
        speedsum = speed/self.wheel_diameter*(720/pi)
        speeddif = 2*self.axle_track/self.wheel_diameter*steering
        self.left_motor.run((speedsum+speeddif)/2)
        self.right_motor.run((speedsum-speeddif)/2)

    def stop(self, stop_type=Stop.coast):
        self.left_motor.stop(stop_type)
        self.right_motor.stop(stop_type)
