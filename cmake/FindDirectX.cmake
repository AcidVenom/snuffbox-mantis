IF (WIN32)
  GET_FILENAME_COMPONENT(WIN10_SDK_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Microsoft SDKs\\Windows\\v10.0;InstallationFolder]" ABSOLUTE CACHE)
  SET (WIN81_SDK_PATH "C:/Program Files (x86)/Windows Kits/8.1")

  FIND_PATH(DirectX_D3D11_INCLUDE_DIR
    NAMES "d3d11.h"
    PATHS
    "${WIN10_SDK_PATH}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um"
    "${WIN81_SDK_PATH}/Include/um"
  )

  FIND_PATH(DirectX_D3D12_INCLUDE_DIR
    NAMES "d3d12.h"
    PATHS
    "${WIN10_SDK_PATH}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um"
  )

  FIND_PATH(DirectX_DXGI_INCLUDE_DIR
    NAMES "dxgi.h"
    PATHS
    "${WIN10_SDK_PATH}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/shared"
    "${WIN81_SDK_PATH}/Include/shared"
  )

  IF (CMAKE_CL_64)
    SET (WIN10_SDK_LIB_PATH "${WIN10_SDK_PATH}/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/x64")
    SET (WIN81_SDK_LIB_PATH "${WIN81_SDK_PATH}/Lib/*/x64")
  ELSE ()
    SET (WIN10_SDK_LIB_PATH "${WIN10_SDK_PATH}/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/x86")
    SET (WIN81_SDK_LIB_PATH "${WIN81_SDK_PATH}/Lib/*/x86")
  ENDIF ()

  IF (DirectX_D3D11_INCLUDE_DIR)
    FIND_LIBRARY(DirectX_D3D11_LIBRARY NAMES d3d11.lib PATHS ${WIN10_SDK_LIB_PATH} ${WIN81_SDK_LIB_PATH})
  ENDIF ()

  IF (DirectX_D3D12_INCLUDE_DIR)
    FIND_LIBRARY(DirectX_D3D12_LIBRARY NAMES d3d12.lib PATHS ${WIN10_SDK_LIB_PATH})
  ENDIF ()


  FIND_LIBRARY(DirectX_DXGI_LIBRARY NAMES dxgi.lib PATHS ${WIN10_SDK_LIB_PATH} ${WIN81_SDK_LIB_PATH})
  FIND_LIBRARY(DirectX_D3DCOMPILER_LIBRARY NAMES d3dcompiler.lib PATHS ${WIN10_SDK_LIB_PATH} ${WIN81_SDK_LIB_PATH})

  IF (DirectX_D3D11_LIBRARY AND DirectX_DXGI_LIBRARY AND DirectX_D3DCOMPILER_LIBRARY)
    SET (DirectX_D3D11_FOUND TRUE)
    SET (DirectX_D3D11_LIBRARIES ${DirectX_D3D11_LIBRARY} ${DirectX_DXGI_LIBRARY} ${DirectX_D3DCOMPILER_LIBRARY})
    MESSAGE (STATUS "Found Direct3D 11: ${DirectX_D3D11_INCLUDE_DIR}")
  ELSE ()
    SET (DirectX_D3D11_FOUND FALSE)
    MESSAGE (WARNING "Could not find D3D11")
  ENDIF ()

  IF (DirectX_D3D12_LIBRARY AND DirectX_DXGI_LIBRARY AND DirectX_D3DCOMPILER_LIBRARY)
    SET (DirectX_D3D12_FOUND TRUE)
    SET (DirectX_D3D12_LIBRARIES ${DirectX_D3D12_LIBRARY} ${DirectX_DXGI_LIBRARY} ${DirectX_D3DCOMPILER_LIBRARY})
    MESSAGE (STATUS "Found Direct3D 12: ${DirectX_D3D12_INCLUDE_DIR}")
  ELSE ()
    SET (DirectX_D3D12_FOUND FALSE)
    MESSAGE (WARNING "Could not find D3D11")
  ENDIF ()

  IF (NOT DirectX_D3D11_FOUND AND NOT DirectX_D3D12_FOUND)
    SET (DirectX_FOUND FALSE)
    IF (DirectX_FIND_REQUIRED)
      MESSAGE (FATAL_ERROR "Could not find any DirectX version that comes with the Windows SDK on this system")
    ENDIF ()
  ELSE ()
    SET (DirectX_FOUND TRUE)
  ENDIF ()
ELSE ()
  MESSAGE(FATAL_ERROR "DirectX is only available on the Win32 platform")
ENDIF ()