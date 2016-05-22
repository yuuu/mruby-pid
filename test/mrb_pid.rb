##
## PID Test
##
assert("PID#target") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
  assert_equal(50, @pid.target)
end

assert("PID#target=") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
	@pid.target = 51
  assert_equal(51, @pid.target)
end


assert("PID#kp") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
  assert_equal(1, @pid.kp)
end

assert("PID#kp=") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
	@pid.kp = 4
  assert_equal(4, @pid.kp)
end

assert("PID#ki") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
  assert_equal(2, @pid.ki)
end

assert("PID#ki=") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
	@pid.ki = 5
  assert_equal(5, @pid.ki)
end

assert("PID#kd") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
  assert_equal(3, @pid.kd)
end

assert("PID#kd=") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
	@pid.kd = 6
  assert_equal(6, @pid.kd)
end

assert("PID#sampling") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
  assert_equal(4, @pid.sampling)
end

assert("PID#sampling=") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
	@pid.sampling = 7
  assert_equal(7, @pid.sampling)
end

assert("PID#max") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
  assert_equal(100, @pid.max)
end

assert("PID#max=") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
	@pid.max = 101 
  assert_equal(101, @pid.max)
end

assert("PID#min") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
  assert_equal(-100, @pid.min)
end

assert("PID#min=") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
	@pid.min = -101 
  assert_equal(-101, @pid.min)
end

assert("PID#calculate") do
  @pid = PID.new(target:50, kp:1, ki:2, kd:3, sampling:4, max:100, min:-100)
  assert_equal(2 + 0 + 16, @pid.calculate(52))
  assert_equal(6 + 3 + 64, @pid.calculate(56))
  assert_equal(-2 - 6 + 48, @pid.calculate(48))
end

