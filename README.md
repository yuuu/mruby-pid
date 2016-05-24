# mruby-pid   [![Build Status](https://travis-ci.org/yuuu/mruby-pid.svg?branch=master)](https://travis-ci.org/yuuu/mruby-pid)
PID class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'yuuu/mruby-pid'
end
```
## example
```ruby
@pid = PID.new(target:50, kp:1.0, ki:0.001, kd:0.2, sampling:4, max:100, min:-100)
#=> #<PID:0x1155fc0>
@pid.calculate(52)
#=> -2.008
@pid.calculate(51)
#=> -0.962
@pid.target = 30
#=> 30
@pid.calculate(50)
#=> -20.042
```

## License
under the MIT License:
- see LICENSE file
