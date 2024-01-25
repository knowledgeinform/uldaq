{
    "targets": [{
        "target_name": "uldaq",
        "sources": [ "index.cpp" ],
        'include_dirs': ["<!(node -p \"require('node-addon-api').include_dir\")"],
        'cflags!': [ '-fno-exceptions' ],
        'cflags_cc!': [ '-fno-exceptions' ],
        'conditions': [
            ["OS=='win'", {
                "defines": [
                    "_HAS_EXCEPTIONS=1",
                    "_WIN32",
                ],
                "libraries": [
                    "C:\\Users\\Public\\Documents\\Measurement Computing\\DAQ\\CWin\\cbw32.lib",
                    "C:\\Users\\Public\\Documents\\Measurement Computing\\DAQ\\CWin\\cbw64.lib"
                ],
                "include_dirs": [
                    "C:\\Users\\Public\\Documents\\Measurement Computing\\DAQ\\CWin\\CWin"
                ],
                "copies": [
                    {
                        "destination": "<(module_root_dir)/build/Release/",
                        "files": [
                            "C:\\Program Files (x86)\\Measurement Computing\\DAQ\\*.dll"
                        ]
                    }
                ],
                "msvs_settings": {
                    "VCCLCompilerTool": {
                        "ExceptionHandling": 1
                    },
                },
            }],
            ["OS=='mac'", {
                'libraries': [ '-luldaq' ],
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                    'CLANG_CXX_LIBRARY': 'libc++',
                    'MACOSX_DEPLOYMENT_TARGET': '10.7',
            },
            }],
        ],
    }],
}