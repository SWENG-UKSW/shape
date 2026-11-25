#include "shape_api.h"
#include "plotter.h"

const uint32_t ALBUM_NUMBER = 0; // wpisz swoj numer albumu

PlotData plot_data;

int shape_api_version(void)
{
    return SHAPE_API_VERSION;
}

uint32_t shape_album_number(void)
{
    return ALBUM_NUMBER;
}

void onPlotterAttached(int face, const PlotArea* area)
{
    plot_data.pix_x = area->pix_x;
    plot_data.pix_y = area->pix_y;
    plot_data.pad_x = area->pad_x;
    plot_data.pad_y = area->pad_y;
    plot_data.range_x_min = (float)area->range_x_min;
    plot_data.range_x_max = (float)area->range_x_max;
    plot_data.range_y_min = (float)area->range_y_min;
    plot_data.range_y_max = (float)area->range_y_max;
    plot_data.xs.clear();
    plot_data.ys.clear();
    GenerateEmptyPlot("plot.png");
}

void onPointsChanged(int face, const Vec2* pts, int n)
{
    plot_data.xs.clear();
    plot_data.ys.clear();
    for (int i = 0; i < n; i++)
    {
        plot_data.xs.push_back(pts[i].x);
        plot_data.ys.push_back(pts[i].y);
    }
}

void onRangeChanged(int face, int has, const double range_u[2])
{
    // Zakres ustawiony klawiszem X sluzy tylko do podgladu na ploterze.
    // Niczego nie wpisujemy do plot_data. W computeVolume zakres jest
    // dostepny jako faces[...].has_range_u i faces[...].range_u.
}

void on_key_f_pressed(void)
{
}

void on_key_g_pressed(void)
{
}

double computeVolume(const FacePoints* faces, int n_faces, double width, double depth)
{
    // Licz wylacznie z parametrow tej funkcji: faces, n_faces, width, depth.
    // Kazde wywolanie moze dotyczyc innej (przeksztalconej) bryly, wiec nie
    // korzystaj z plot_data ani z wartosci zapamietanych wczesniej.
    const FacePoints& front = faces[FACE_FRONT]; // punkty sciany FRONT tego wywolania
    const double integration_from = 0.0;         // granice calkowania: cala szerokosc bryly
    const double integration_to = width;
    const double solid_depth = depth;

    return 0.0;
}
