# zerr
This package was created using the Min-DevKit for Max, an API and supporting tools for writing externals in modern C++. 

## Features

- Real-time error notification and logging with configurable verbosity levels
- Visual error indicators that integrate seamlessly with the Max interface
- Detailed error reporting with stack traces for advanced debugging
- Error filtering options to focus on specific types of issues
- Integration with Max's existing error handling systems

## Prerequisites

You can use the objects provided in this package as-is. To code your own objects, or to re-compile existing objects, you will need a compiler:

- On the Mac this means **Xcode 9 or later** (you can get from the App Store for free).
- On Windows this means **Visual Studio 2017** (you can download a free version from Microsoft). The installer for Visual Studio 2017 offers an option to install Git, which you should choose to do. You will also need the Min-DevKit, available from the Package Manager inside of Max or directly from Github.

## Installation

1. Download the latest release from the releases page
2. Unzip the package
3. Place the entire folder in your Max Packages directory:
   - macOS: `~/Documents/Max 8/Packages`
   - Windows: `C:\Users\[username]\Documents\Max 8\Packages`
4. Restart Max if it's already running

## Usage

The package includes the following objects:

- `zerr.track` - Monitors errors in connected objects and reports them
- `zerr.log` - Saves error histories to disk for later analysis
- `zerr.filter` - Allows selective filtering of error types
- `zerr.display` - Creates a visual interface for error monitoring

For detailed documentation on each object, see the help patches included in the package.

## Examples

Several example patches are included in the `examples` folder to demonstrate common use cases and advanced configurations.

## Contributors / Acknowledgements

The zerr is the work of some amazing and creative artists, researchers, and coders. Special thanks to:

- The Cycling '74 team for creating and maintaining the Max environment
- The Min-DevKit developers for providing the underlying architecture
- All beta testers who helped identify and resolve issues

## Version History

- v1.0.0 (2025-04-15): Initial release
- v1.0.1 (2025-05-01): Bug fixes and performance improvements

## Support

For support, please contact the developer of this package. You can also:

- Open an issue on our GitHub repository
- Join our discussion forum at [forum link]
- Check the documentation at [documentation link]

## License

This software is licensed under the MIT License. See LICENSE.txt for details.
