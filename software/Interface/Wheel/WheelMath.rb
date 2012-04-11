# %% Wheel designations: 

# % 1: back-left
# % 2: middle-left
# % 3: front-left
# % 4: back-right
# % 5: middle-right
# % 6: front-right

# %% constants
def setup_constants
	@b = 0.381
	@w = 0.7396/2
	@R = 0.232
	@vMax = 1.3
	@cMax = 1.0/@w - 0.001
	@thetaMax = (3.14159/180)*140
end

def input_joystick
	#@right_joystick_percent = -75.0
	#@left_joystick_percent = 100.0
	#@right_joystick_percent = @hs1.value
	#@left_joystick_percent = @hs2.value
	@left_axis_x =   @js.axis[0]/327.67
	@left_axis_y =  -@js.axis[1]/327.67
	@right_axis_x =  @js.axis[3]/327.67
	@right_axis_y =  -@js.axis[4]/327.67

end

#Ackerman (Explicit) steering mode is selected:
def explicit
	input_joystick
#outputs (target values)
	c = (@right_axis_x/100.0)*@cMax
	v = []
	v[0] = (@left_axis_y/100.0)*@vMax
	r = 1.0/c
#(radians) steering angle of wheel 1
	theta = []
	theta[1] = atan(@b/(r+@w))
	theta[2] = 0
	theta[3] = -theta[1]
	theta[4] = atan(@b/(r-@w))
	theta[5] = 0
	theta[6] = -theta[4]


#(m/s) linear velocity of drive motor 1
	
	v[2] = v[0]*(r+@w)/r
	v[5] = v[0]*(r-@w)/r
	v[1] = v[2]/cos(theta[3])
	v[3] = v[1]
	v[4] = v[5]/cos(theta[4])
	v[6] = v[4]
	
	if c == 0
		v[1] = v[2] = v[3] = v[4] = v[5] = v[6] = v[0]
	end

	velocity_scale v

#(rad/s) rotation rate of drive motor 1
	omega = []
	omega[1] = v[1]/@R
	omega[2] = v[2]/@R
	omega[3] = omega[1]
	omega[4] = v[4]/@R
	omega[5] = v[5]/@R
	omega[6] = omega[4]


	@wheels[0].angle = theta[1]
	@wheels[1].angle = theta[2]
	@wheels[2].angle = theta[3]
	@wheels[3].angle = theta[4]
	@wheels[4].angle = theta[5]
	@wheels[5].angle = theta[6]
	@wheels[0].velocity  = v[1]
	@wheels[1].velocity  = v[2]
	@wheels[2].velocity  = v[3]
	@wheels[3].velocity  = v[4]
	@wheels[4].velocity  = v[5]
	@wheels[5].velocity  = v[6]
	@wheels[0].omega = omega[1]
	@wheels[1].omega = omega[2]
	@wheels[2].omega = omega[3]
	@wheels[3].omega = omega[4]
	@wheels[4].omega = omega[5]
	@wheels[5].omega = omega[6]
end

#Vector (Crab) steering mode is selected:
def vector
	input_joystick

#(radians) steering angle of all wheels
	#theta = (@right_axis_y/100.0)*@thetaMax
	theta = atan2(@right_axis_y,@right_axis_x) - Math::PI/2
#(m/s) linear velocity of all drive wheels
	v = (@left_axis_y/100.0)*@vMax
	velocity_scale v
	#v = ((@right_axis_y/100)**2.0+(@right_axis_x/100)**2.0)**0.5*@vMax
	#v = @vMax if v > @vMax
#(radians/s) rotation rate of all drive motors
	omega = v/@R

	@wheels[0].angle = theta
	@wheels[1].angle = theta
	@wheels[2].angle = theta
	@wheels[3].angle = theta
	@wheels[4].angle = theta
	@wheels[5].angle = theta
	@wheels[0].velocity  = v
	@wheels[1].velocity  = v
	@wheels[2].velocity  = v
	@wheels[3].velocity  = v
	@wheels[4].velocity  = v
	@wheels[5].velocity  = v
	@wheels[0].omega = omega
	@wheels[1].omega = omega
	@wheels[2].omega = omega
	@wheels[3].omega = omega
	@wheels[4].omega = omega
	@wheels[5].omega = omega
end


# %% Zero-Radius (In-Place) steering mode is selected:
def zeroRadius
	input_joystick


#(radians) steering angle of wheel 1
	theta = []
	theta[1] = atan(@b/@w)
	theta[2] = 0
	theta[3] = -theta[1]
	theta[4] = -theta[1]
	theta[5] = 0
	theta[6] = theta[1]
    

#(m/s) linear velocity of drive wheel 1
	v[1] = (@left_axis_x/100.0)*@vMax*0.5
	v[2] = v[1]*(@w/(@b**2 + @w**2)**0.5)
	v[3] = v[1]
	v[4] = -v[1]
	v[5] = -v[2]
	v[6] = -v[1]

	velocity_scale v
    

#(radians/s) rotation rate of drive motor 1
	omega = []
	omega[1] = v[1]/@R
	omega[2] = v[2]/@R
	omega[3] = v[3]/@R
	omega[4] = v[4]/@R
	omega[5] = v[5]/@R
	omega[6] = v[6]/@R


	@wheels[0].angle = theta[1]
	@wheels[1].angle = theta[2]
	@wheels[2].angle = theta[3]
	@wheels[3].angle = theta[4]
	@wheels[4].angle = theta[5]
	@wheels[5].angle = theta[6]
	@wheels[0].velocity  = v[1]
	@wheels[1].velocity  = v[2]
	@wheels[2].velocity  = v[3]
	@wheels[3].velocity  = v[4]
	@wheels[4].velocity  = v[5]
	@wheels[5].velocity  = v[6]
	@wheels[0].omega = omega[1]
	@wheels[1].omega = omega[2]
	@wheels[2].omega = omega[3]
	@wheels[3].omega = omega[4]
	@wheels[4].omega = omega[5]
	@wheels[5].omega = omega[6]
end

def velocity_scale v
	max = v.sort.last
	return if max < @vMax
	scale = @vMax/max
	v.map! {|x|x*scale}
end