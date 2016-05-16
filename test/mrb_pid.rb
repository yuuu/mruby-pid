##
## PID Test
##

assert("PID#hello") do
  t = PID.new "hello"
  assert_equal("hello", t.hello)
end

assert("PID#bye") do
  t = PID.new "hello"
  assert_equal("hello bye", t.bye)
end

assert("PID.hi") do
  assert_equal("hi!!", PID.hi)
end
