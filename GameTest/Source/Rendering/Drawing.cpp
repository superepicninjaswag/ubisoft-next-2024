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
    if(top.y < mid.y)
    {
        std::swap(top, mid);
    }
    if(top.y < bot.y)
    {
        std::swap(top, bot);
    }
    if(mid.y < bot.y)
    {
        std::swap(mid, bot);
    }

    // Draw outline of triangle
    App::DrawLine(bot.x, bot.y, mid.x, mid.y, c.r, c.g, c.b);
    App::DrawLine(mid.x, mid.y, top.x, top.y, c.r, c.g, c.b);
    App::DrawLine(top.x, top.y, bot.x, bot.y, c.r, c.g, c.b);

    float heightBotTop = top.y - bot.y + 1;
    // Draw bottom half of triangle
    float heightBotMid = mid.y - bot.y;
    if (heightBotMid > 0)
    {
        for (float y = bot.y; y <= mid.y; y = y + 1.0f)
        {
            float coverageBotTop = (y - bot.y)/heightBotTop;
            float coverageBotMid = (y - bot.y)/heightBotMid;

            if (coverageBotMid > 1 || coverageBotTop > 1)
            {
                break;
            }

            Vec3 pointBotTop = bot * (1 - coverageBotTop) + top * coverageBotTop;
            Vec3 pointBotMid = bot * (1 - coverageBotMid) + mid * coverageBotMid;

            App::DrawLine(floorf(pointBotMid.x) + 0.5f, y, ceilf(pointBotTop.x) - 0.5f, y, c.r, c.g, c.b);
        }
    }

    // Draw bottom half of triangle
    float heightMidTop = top.y - mid.y;
    if (heightMidTop > 0)
    {
        for (float y = mid.y; y <= top.y; y = y + 1.0f)
        {
            float coverageBotTop = (y - bot.y)/heightBotTop;
            float coverageMidTop = (y - mid.y)/heightMidTop;

            if (coverageMidTop > 1 || coverageBotTop > 1)
            {
                break;
            }

            Vec3 pointBotTop = bot * (1 - coverageBotTop) + top * coverageBotTop;
            Vec3 pointMidTop = mid * (1 - coverageMidTop) + top * coverageMidTop;

            App::DrawLine(floorf(pointMidTop.x) + 0.5f, y, ceilf(pointBotTop.x) - 0.5f, y, c.r, c.g, c.b);
        }
    }
}