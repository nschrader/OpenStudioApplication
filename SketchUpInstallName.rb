def fixup(file, rpath_replacement)
  puts
  puts file
  `install_name_tool -add_rpath #{rpath_replacement} #{file}`
  #`otool -L #{file}`.split("\n").each do |line|
  #  line.strip!
  #  if md = /(@rpath[^\s]*)/.match(line)
  #    line = md[1]
  #    new_line = line.gsub("@rpath", rpath_replacement)
  #    if md = /Qt.*\/(Qt.*?)$/.match(new_line)
  #    	#new_line = new_line.gsub(md[0], md[1])
  #    end
  #    puts "#{line} -> #{new_line}"
  #   `install_name_tool -change #{line} #{new_line} #{file}`
  #  end
  #end
end

# Fix up any load paths in openstudio_modeleditor.bundle
def fixup_model_editor(lib_path, lib_path_replacemant, openstudio_modeleditor_path)

 puts "#{lib_path} -> #{lib_path_replacemant} in #{openstudio_modeleditor_path}"
      `install_name_tool -change #{lib_path} #{lib_path_replacemant} #{openstudio_modeleditor_path}`
end

dir = File.dirname(ARGV[0])

fixup(ARGV[0], "@loader_path/")

qt_libs = ['QtConcurrent', 'QtCore', 'QtGui', 'QtNetwork','QtPrintSupport', 'QtWidgets', 'QtXml']
qt_libs.each do |qt_lib|
  Dir.glob(File.join(dir, '**', qt_lib)).each do |file|
    next if /Headers/.match(file)
    fixup(file, "@loader_path/")
  end
end

fixup(File.join(dir, "platforms/libqcocoa.dylib"), "@loader_path/..")

fixup_model_editor("libopenstudiolib.dylib", "@loader_path/libopenstudiolib.dylib", File.join(dir, "openstudio_modeleditor.bundle"))
