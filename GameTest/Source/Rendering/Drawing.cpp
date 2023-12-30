#include "stdafx.h"

#include "Drawing.h"


void drawTriangle(const Face &triangle, const Colour c) {
    for (int i = 0; i < 3; i++)
    {
        App::DrawLine(triangle.points[i].x, triangle.points[i].y, triangle.points[(i + 1) % 3].x, triangle.points[(i + 1) % 3].y, c.r, c.g, c.b);
    }
}

void drawFilledTriangle(const Face &triangle, const Colour c) {
    Vec3 top = triangle.points[0];
    Vec3 mid = triangle.points[1];
    Vec3 bot = triangle.points[2];

    // Ignore degen triangles
    if (top.y == mid.y && mid.y == bot.y)
    {
        return;
    }
    if (top.x == mid.x && mid.x == bot.x)
    {
        return;
    }

    // Sort the three points of the triangle according to their y component
    if (top.y < mid.y)
    {
        std::swap(top, mid);
    }
    if (top.y < bot.y)
    {
        std::swap(top, bot);
    }
    if (mid.y < bot.y)
    {
        std::swap(mid, bot);
    }

    float heightBottomToTop = top.y - bot.y;
    float heightBottomToMid = mid.y - bot.y + 1.0f;
    float heightMidToTop = top.y - mid.y + 1.0f;

    // Bottom half
    Vec3 dirBottomToTop = top - bot;
    Vec3 dirBottomToMid = mid - bot;
    Vec3 dirMidToTop = top - mid;

    for (float y = bot.y; y <= mid.y; y = y + 1.0f)
    {
        float coverageBotToTop = (y - bot.y) / heightBottomToTop;
        float coverageBotToMid = (y - bot.y) / heightBottomToMid;

        Vec3 a = bot + dirBottomToTop * coverageBotToTop;
        Vec3 b = bot + dirBottomToMid * coverageBotToMid;

        if (a.x > b.x)
        {
            std::swap(a, b);
        }

        App::DrawLine(a.x, y, b.x, y, c.r, c.g, c.b);
    }

    // Top half
    for (float y = mid.y; y <= top.y; y = y + 1.0f)
    {
        float coverageBotToTop = (y - bot.y) / heightBottomToTop;
        float coverageMidToTop = (y - mid.y) / heightMidToTop;

        Vec3 a = bot + dirBottomToTop * coverageBotToTop;
        Vec3 b = mid + dirMidToTop * coverageMidToTop;

        if (a.x > b.x)
        {
            std::swap(a, b);
        }

        App::DrawLine(a.x, y, b.x, y, c.r, c.g, c.b);
    }
}