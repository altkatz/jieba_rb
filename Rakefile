require "bundler/gem_tasks"
require 'rake/testtask'
require 'rake/extensiontask'

Rake::ExtensionTask.new "jieba"

Rake::TestTask.new do |t|
  t.libs << 'test'
end

task :default => :test
