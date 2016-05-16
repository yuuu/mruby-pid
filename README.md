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
p PID.hi
#=> "hi!!"
t = PID.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
