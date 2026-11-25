#ifndef SHAPE_API_H
#define SHAPE_API_H

#include <cstdint>

#define SHAPE_API_VERSION 2

#if defined(_WIN32)
#define SHAPE_EXPORT __declspec(dllexport)
#else
#define SHAPE_EXPORT __attribute__((visibility("default")))
#endif

// Sciany plaskie bryly. Wspolrzedne (u, v) sciany: poczatek w lewym dolnym
// narozniku sciany ogladanej z zewnatrz, u w prawo, v w gore.
enum FaceId {
    FACE_FRONT = 0,  // y = 0, widok z -y: u = x,     v = z
    FACE_BACK = 1,   // y = D, widok z +y: u = W - x, v = z
    FACE_LEFT = 2,   // x = 0, widok z -x: u = D - y, v = z
    FACE_RIGHT = 3,  // x = W, widok z +x: u = y,     v = z
    FACE_BOTTOM = 4  // z = 0, widok z -z, gora obrazu = +y: u = W - x, v = y
};

struct Vec2 {
    double x, y;
};

struct FacePoints {
    int face;
    const Vec2* pts;
    int n;
    int has_range_u;    // 1 gdy zakres u ustawiony klawiszem X
    double range_u[2];  // range_u[0] <= range_u[1]
};

struct PlotArea {
    int pix_x, pix_y, pad_x, pad_y;
    double range_x_min, range_x_max, range_y_min, range_y_max;
};

static_assert(sizeof(Vec2) == 16, "Vec2 layout");

extern "C" {

// Zwraca SHAPE_API_VERSION.
SHAPE_EXPORT int shape_api_version(void);

// Zwraca ALBUM_NUMBER z gory student.cpp.
SHAPE_EXPORT uint32_t shape_album_number(void);

// Ploter zaczepiony na scianie face; po powrocie exe wczytuje plot.png.
SHAPE_EXPORT void onPlotterAttached(int face, const PlotArea* area);

// Punkty zaczepionej sciany po kazdej zmianie (dodanie, usuniecie, C).
SHAPE_EXPORT void onPointsChanged(int face, const Vec2* pts, int n);

// Zakres u ustawiony klawiszem X na zaczepionej scianie (has = 0: skasowany).
SHAPE_EXPORT void onRangeChanged(int face, int has, const double range_u[2]);

// Klawisze studenta; po powrocie exe wczytuje plot.png.
SHAPE_EXPORT void on_key_f_pressed(void);
SHAPE_EXPORT void on_key_g_pressed(void);

// faces: zawsze 5 wpisow w kolejnosci FaceId, n moze byc 0. width = W,
// depth = D bryly, ktorej dotycza punkty (pod T wymiary bryly po
// przeksztalceniu).
SHAPE_EXPORT double computeVolume(const FacePoints* faces, int n_faces,
                                  double width, double depth);

} // extern "C"

#endif // SHAPE_API_H
