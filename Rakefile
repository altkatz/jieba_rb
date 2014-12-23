require "bundler/gem_tasks"
require 'rake/testtask'
require 'rake/extensiontask'

Rake::ExtensionTask.new "jieba"

Rake::TestTask.new do |t|
  t.libs << 'test'
end
desc "clean compile files"
task :clean_compile do
  system "rm -r tmp"
  system "rm lib/*.bundle"
end
task :default => :test
