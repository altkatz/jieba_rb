require "bundler/gem_tasks"
require 'rake/testtask'
require 'rake/extensiontask'

gem = Gem::Specification.load(File.dirname(__FILE__) + '/jieba_rb.gemspec')
Rake::ExtensionTask.new('jieba_rb', gem) do |ext|
  ext.lib_dir = File.join('lib', 'jieba_rb')
end

Rake::Task[:test].prerequisites << :compile

Rake::TestTask.new do |t|
  t.libs << 'test'
end
desc "clean compile files"
task :clean_compile do
  system "rm -r tmp"
  system "rm lib/*.bundle"
end
task :default => :test
