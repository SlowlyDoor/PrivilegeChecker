{
  "targets": [
    {
      "target_name": "addon",
      "sources": ["src/checkPrivileges.cc"],
      "include_dirs": [
        "<!(node -p \"require('path').dirname(require.resolve('node-addon-api'))\")",
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
      "libraries": ["netapi32.lib"]
    }
  ]
}
