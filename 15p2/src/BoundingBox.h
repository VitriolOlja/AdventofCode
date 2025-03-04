#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

typedef struct BoundingBox {
    int m_x;       // X position
    int m_y;       // Y position
    int width;     // Width of the bounding box
    int height;    // Height of the bounding box
} BoundingBox;

bool collidesWith(const BoundingBox& a, const BoundingBox& b) {
    // Check if the bounding boxes overlap on the x-axis and y-axis
    bool xOverlap = (a.m_x + a.width > b.m_x) && (a.m_x < b.m_x + b.width);
    bool yOverlap = (a.m_y + a.height > b.m_y) && (a.m_y < b.m_y + b.height);

    return xOverlap && yOverlap;
}

#endif