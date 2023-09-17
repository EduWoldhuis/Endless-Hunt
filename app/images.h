#pragma once
// #include "../imgui/d3dx9tex.h"

#include <d3d9.h>
#include <d3dx9tex.h>
#pragma comment(lib, "D3dx9")
// For res:https://learn.microsoft.com/en-us/windows/win32/wic/-wic-bitmapsources-howto-loadfromresource


// Simple helper function to load an image into a DX9 texture with common settings
 
bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height, LPDIRECT3DDEVICE9 device)
{
    // Load texture from disk
    PDIRECT3DTEXTURE9 texture;
    HRESULT hr = D3DXCreateTextureFromFileA(device, filename, &texture);
    // HRESULT hr = D3DXCreateTextureFromResourceA(device, test, &texture)  For resource stuff later
    if (hr != S_OK)
        return false;

    // Retrieve description of the texture surface so we can access its size
    D3DSURFACE_DESC my_image_desc;
    texture->GetLevelDesc(0, &my_image_desc);

    *out_texture = texture;
    *out_width = (int)my_image_desc.Width;
    *out_height = (int)my_image_desc.Height;
    return true;
}

/*
// from disk
status = ::D3DXCreateTextureFromFileA( device, "C:\\image.jpg", &p_texture);
// from memory
status = ::D3DXCreateTextureFromFileInMemory( device, g_image.data(), g_image.size(), &p_texture );

if ( status != D3D_OK )
{
    __debugbreak();
}

D3DSURFACE_DESC surface_desc{};

status = p_texture->GetLevelDesc( 0u, &surface_desc );
*/
/*
bool LoadTextureFromMemory(unsigned char* asset, PDIRECT3DTEXTURE9* out_texture, int out_width, int out_height, LPDIRECT3DDEVICE9* device)
{
    D3DXCreateTextureFromFileInMemoryEx(device, &asset, sizeof(asset), out_width, out_height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &out_texture);
    return true;
}
*/



