#pragma once

#include <SFML/Graphics.hpp>

struct Bound_box {
    enum class Bound_type { 
        SOLID,      
        PLATFORM,   
        HAZARD,     
        WATER_PLATFORM,  
        LAVA_PLATFORM,   
        POISON_PLATFORM, 
        bl_rib,     
        bl_red,     
        bl_blue,    
        bl_green,   
        bl_r_door,  
        bl_b_door   
    };

    Bound_box(sf::Vector2f point1_, sf::Vector2f point2_, Bound_type type_) :
        point1(point1_), point2(point2_), type(type_) {}

    bool inside(sf::Vector2f point) const {
        return (point.x >= point1.x && point.x <= point2.x &&
                point.y >= point1.y && point.y <= point2.y);
    }

    sf::Vector2f point1;  
    sf::Vector2f point2;  
    Bound_type type;
};

struct Bound_polygon {
    enum class Bound_type {
        SOLID,      
        PLATFORM,   
        HAZARD,     
        bl_rib,     
        bl_red,     
        bl_blue,    
        bl_green,   
        bl_r_door,  
        bl_b_door   
    };

    Bound_polygon(const std::vector<sf::Vector2f>& points_, Bound_type type_) :
        points(points_), type(type_) {}

    bool inside(sf::Vector2f point) const {
        
        bool inside = false;
        size_t j = points.size() - 1;
        
        for (size_t i = 0; i < points.size(); i++) {
            if (((points[i].y > point.y) != (points[j].y > point.y)) &&
                (point.x < (points[j].x - points[i].x) * (point.y - points[i].y) /
                (points[j].y - points[i].y) + points[i].x)) {
                inside = !inside;
            }
            j = i;
        }
        return inside;
    }

    std::vector<sf::Vector2f> points;
    Bound_type type;
};
