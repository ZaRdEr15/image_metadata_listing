# Remove the linker if it exists
if [ -L ${img_metadata} ]; then
  rm -f img_metadata
fi

# Build the application
make

# Create a linker to the executable
ln -s build/img_metadata img_metadata