require "mkmf"
abs = File.expand_path File.dirname(__FILE__)

LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']

HEADER_DIRS = [
  INCLUDEDIR,
  "#{abs}/../cppjieba/src"
]

LIB_DIRS = [

  LIBDIR
]

dir_config('cppjieba_src', HEADER_DIRS, LIB_DIRS)

CONFIG["CXXFLAGS"] += " -std=c++0x -O3"
$CXXFLAGS = "#{$CXXFLAGS} -std=c++0x -O3"
create_makefile 'jieba'
# respect header changes
headers = Dir.glob('*.{hpp,h}').join ' '
File.open 'Makefile', 'a' do |f|
  f.puts "\n$(OBJS): #{headers}"
end
