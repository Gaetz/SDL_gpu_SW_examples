# SHADERCROSS Documentation:
# Usage: shadercross <input> [options]
# Required options:
#  -s | --source <value>            Source language format. May be inferred from the filename. Values: [SPIRV, HLSL]
#  -d | --dest <value>              Destination format. May be inferred from the filename. Values: [DXBC, DXIL, MSL, SPIRV, HLSL, JSON]
#  -t | --stage <value>             Shader stage. May be inferred from the filename. Values: [vertex, fragment, compute]
#  -e | --entrypoint <value>        Entrypoint function name. Default: "main".
#  -o | --output <value>            Output file.
#
# Optional options:
#  -I | --include <value>           HLSL include directory. Only used with HLSL source.
#  -D<name>[=<value>]               HLSL define. Only used with HLSL source. Can be repeated.
#                                   If =<value> is omitted the define will be treated as equal to 1.
#  --msl-version <value>            Target MSL version. Only used when transpiling to MSL. The default is 1.2.0.
#  -g | --debug                     Generate debug information when possible. Shaders are valid only when graphics debug

# Compile SPIRV to different destinations

# Define source and destination directories
SOURCE_DIR="./Source"
SPIRV_DIR="./Compiled/SPIRV"
HLSL_DIR="./Compiled/HLSL"
MSL_DIR="./Compiled/MSL"

echo "-- Transpiling shaders"

# Create destination directories if they don't exist
mkdir -p "$SPIRV_DIR" "$HLSL_DIR" "$MSL_DIR"

# Step 1: Compile all .vert and .frag files to SPIRV using Vulkan compiler
for shader_file in "$SOURCE_DIR"/*.vert "$SOURCE_DIR"/*.frag; do
    if [ -f "$shader_file" ]; then
        filename=$(basename -- "$shader_file")
        spirv_file="$SPIRV_DIR/${filename}.spv"
        glslc "$shader_file" -o "$spirv_file"
        echo "Compiled $shader_file to SPIRV: $spirv_file"
    fi
done

# Step 2: Validate SPIR-V files using spirv-val
for spirv_file in "$SPIRV_DIR"/*.spv; do
    if [ -f "$spirv_file" ]; then
        spirv-val "$spirv_file"
        if [ $? -ne 0 ]; then
            echo "Validation failed for $spirv_file. Skipping transpilation."
            continue
        fi
        echo "Validated SPIR-V file: $spirv_file"
    fi
done

# Step 3: Convert SPIRV to HLSL and MSL using shadercross
for spirv_file in "$SPIRV_DIR"/*.spv; do
    if [ -f "$spirv_file" ]; then
        filename=$(basename -- "$spirv_file" .spv)
        extension="${filename##*.}" # Extract the original extension (.vert or .frag)
        base_name="${filename%.*}"  # Remove the extension to get the base name
        hlsl_file="$HLSL_DIR/${base_name}.${extension}.hlsl"
        msl_file="$MSL_DIR/${base_name}.${extension}.msl"

        # Determine shader stage based on the original extension
        if [ "$extension" = "vert" ]; then
            stage="vertex"
        elif [ "$extension" = "frag" ]; then
            stage="fragment"
        else
            echo "Unknown shader type for $spirv_file"s
            continue
        fi

        # Convert to HLSL
        shadercross "$spirv_file" -s SPIRV -d HLSL -t "$stage" -o "$hlsl_file"
        echo "Converted $spirv_file to HLSL: $hlsl_file"

        # Convert to MSL
        shadercross "$spirv_file" -s SPIRV -d MSL -t "$stage" -o "$msl_file"
        echo "Converted $spirv_file to MSL: $msl_file"
    fi
done

# Copy to the cmake build directory
echo "-- Copying compiled shaders to the cmake build directory"
CMAKE_BUILD_DIR="../../cmake-build-debug"
OUTPUT_DIR="$CMAKE_BUILD_DIR/Content/Shaders"
cp -r -v "Compiled" "$OUTPUT_DIR"