//
// Created by acsr on 9/22/22.
//

#ifndef RACECARPLANNER_TEST_MAP_HPP
#define RACECARPLANNER_TEST_MAP_HPP

#include <tinyxml2.h>
#include <string>
#include <iostream>
#include "shape.hpp"
#include <boost/geometry.hpp>

namespace bg = boost::geometry;
typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;
typedef bg::model::polygon<point_t> polygon_t;

namespace acsr{
    class TestMap{
    public:
        TestMap()=default;

        explicit TestMap(const std::string& filename){
            tinyxml2::XMLDocument doc;
            doc.LoadFile( filename.c_str());
            auto root = doc.RootElement();
            for (tinyxml2::XMLElement* child = root->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
            {
                if(strcmp(child->Name(),"rectangle")==0){
                    auto width = child->DoubleAttribute("width");
                    auto height = child->DoubleAttribute("height");
                    auto x = child->DoubleAttribute("x");
                    auto y = child->DoubleAttribute("y");
                    add_rectangle(x,y,width,height);
                }else if(strcmp(child->Name(),"circle")==0){
                    auto x = child->DoubleAttribute("x");
                    auto y = child->DoubleAttribute("y");
                    auto radius = child->DoubleAttribute("radius");
                    add_circle(x,y,radius);
                }

            }
            std::cout<<shapes.size()<<'\n';
        }

        bool valid(double x,double y){
            return std::all_of(std::begin(shapes),std::end(shapes),[x,y](auto &s){
                return !bg::within(point_t {x,y},s);
            });
        }

        std::vector<std::pair<std::vector<double>,std::vector<double>>> plot_data(){
            std::vector<std::pair<std::vector<double>,std::vector<double>>> pts;
            for(auto& shape: shapes) {
                std::vector<double> x,y;
                bg::for_each_point(shape,[&x,&y](point_t& pt){
                    x.push_back(pt.get<0>());
                    y.push_back(pt.get<0>());
                });
                pts.emplace_back(x,y);
            }
            return pts;
            /*
            std::vector<std::pair<std::vector<double>,std::vector<double>>> v(shapes.size());
            std::transform(shapes.begin(),shapes.end(),v.begin(),[](point_t& s){
                return s->get_plot_data();
            });
            return v;*/
        }

        void add_circle(double x,double y,double radius){
            polygon_t poly;
            for(auto i=0;i<101;++i){
                bg::append(poly.outer(), point_t(x+radius*cos(2*M_PI/100*i), y+radius*sin(2*M_PI/100*i)));
            }
            shapes.push_back(poly);
        }

        void add_rectangle(double x,double y,double width,double height){
            polygon_t poly;
            bg::append(poly.outer(), point_t(x, y));
            bg::append(poly.outer(), point_t(x+width, y));
            bg::append(poly.outer(), point_t(x+width, y+height));
            bg::append(poly.outer(), point_t(x, y+height));
            bg::append(poly.outer(), point_t(x, y));
            shapes.push_back(poly);
        }

        void add_polygon(std::initializer_list<point_t> pts){
            shapes.push_back(polygon_t{pts});
        }

    private:
        //std::vector<std::shared_ptr<Shape<double>>> shapes;
        //std::vector<Circle<double>> circles;
        std::vector<polygon_t> shapes;
    };

}

#endif //RACECARPLANNER_TEST_MAP_HPP
