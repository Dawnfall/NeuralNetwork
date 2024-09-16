#include "Utils.h"
#include <iostream>

namespace Utils
{
    // Function to set a pixel color in an SDL_Surface
    void setPixelColor(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        // First check if the coordinates are within bounds
        if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
            std::cerr << "Pixel coordinates are out of bounds!" << std::endl;
            return;
        }

        // Lock the surface if it must be locked (not all surfaces need this)
        if (SDL_MUSTLOCK(surface)) {
            SDL_LockSurface(surface);
        }

        // Get the pixel format of the surface
        Uint32 pixelColor = SDL_MapRGBA(surface->format, r, g, b, a);

        // Calculate the pixel's position in the surface's pixel array
        int bytesPerPixel = surface->format->BytesPerPixel;
        Uint8* pixelPtr = (Uint8*)surface->pixels + y * surface->pitch + x * bytesPerPixel;

        // Set the pixel color depending on the BytesPerPixel
        switch (bytesPerPixel) {
        case 1: // 8-bit color
            *pixelPtr = pixelColor;
            break;
        case 2: // 16-bit color
            *(Uint16*)pixelPtr = pixelColor;
            break;
        case 3: // 24-bit color (RGB)
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                pixelPtr[0] = (pixelColor >> 16) & 0xFF; // Red
                pixelPtr[1] = (pixelColor >> 8) & 0xFF;  // Green
                pixelPtr[2] = pixelColor & 0xFF;         // Blue
            }
            else {
                pixelPtr[0] = pixelColor & 0xFF;         // Blue
                pixelPtr[1] = (pixelColor >> 8) & 0xFF;  // Green
                pixelPtr[2] = (pixelColor >> 16) & 0xFF; // Red
            }
            break;
        case 4: // 32-bit color (RGBA)
            *(Uint32*)pixelPtr = pixelColor;
            break;
        }

        // Unlock the surface if needed
        if (SDL_MUSTLOCK(surface)) {
            SDL_UnlockSurface(surface);
        }
    }


}