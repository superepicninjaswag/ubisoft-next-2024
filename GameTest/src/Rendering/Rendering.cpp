#include "stdafx.h"

#include "Rendering.h"


void drawTriangle(const Face &triangle, const Colour c) {
    App::DrawLine(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, c.r, c.g, c.b);
    App::DrawLine(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, c.r, c.g, c.b);
    App::DrawLine(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x, triangle.points[0].y, c.r, c.g, c.b);
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

    // Draw bounding line to make sure top and bottom end points are drawn
    App::DrawLine(bot.x, bot.y, mid.x, mid.y, c.r, c.g, c.b);
    App::DrawLine(mid.x, mid.y, top.x, top.y, c.r, c.g, c.b);

    float heightBotTop = top.y - bot.y + 1;
    // Draw bottom half of triangle
    float heightBotMid = mid.y - bot.y;
    if (heightBotMid > 0)
    {
        for (int y = bot.y; y <= mid.y; y++)
        {
            float coverageBotTop = (y - (int) bot.y)/heightBotTop;
            float coverageBotMid = (y - (int) bot.y)/heightBotMid;

            if (coverageBotMid > 1)
            {
                break;
            }

            Vec3 pointBotTop = bot + (top - bot) * coverageBotTop;
            Vec3 pointBotMid = bot + (mid - bot) * coverageBotMid;

            App::DrawLine(pointBotMid.x, y, pointBotTop.x, y, c.r, c.g, c.b);
        }
    }

    // Draw bottom half of triangle
    float heightMidTop = top.y - mid.y;
    if (heightMidTop > 0)
    {
        for (int y = mid.y; y <= top.y; y++)
        {
            float coverageBotTop = (y - (int) bot.y)/heightBotTop;
            float coverageMidTop = (y - (int) mid.y)/heightMidTop;

            if (coverageMidTop > 1)
            {
                break;
            }

            Vec3 pointBotTop = bot + (top - bot) * coverageBotTop;
            Vec3 pointMidTop = mid + (top - mid) * coverageMidTop;

            App::DrawLine(pointMidTop.x, y, pointBotTop.x, y, c.r, c.g, c.b);
        }
    }
}