
/*  Author(s) Name:		
*                       Saeed Rayegan
*                       email : sr283100@gmail.com
*                       phone:5144436479
*
*   Description : This code developed to calculate the azimuth angle of each triangle from the .stl geometry file
*
******************************************************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

constexpr auto PI = 3.14159265;
#include "triangle_area_calculator.h"



#include "Point.h"
#include "crossProduct.h"
#include "findMidpoint.h"
#include "calculateClockwiseAngle.h"
#include "isPointInPolygon.h"
#include "calculatePerpendicularPoints.h"

//define name to read input files and print output files
std::ifstream input;
std::ofstream output;

string chr;
double num;

int CS_Nb = 1;
int* tri_num;


double** triangle_azimuth;

int main() {
	int bug_found = 0;//This is a counter for the potential bug in azimuth angle function. The simulation stops if number of wrong azimuth angle for triangles exceeds 5% of all triangles in the geometry file.
   	//Must be omitted when adding to CityBEM. Be careful
	tri_num = new int[CS_Nb];//ommit this later
	tri_num[0] = 93;
	
	//declarations
	triangle_azimuth = new double* [CS_Nb];
	for (int i = 0; i <= CS_Nb - 1; i++) {
		triangle_azimuth[i] = new double[tri_num[i]];
	}
	double* x, * y, * z;//vertices of each triangle
	x = new double[3];
	y = new double[3];
	z = new double[3];
	int b_triangle_id;//this is the counter for numbers of triangles within each solid
	int j = 0;//building number counter in the .stl file

	input.open("Input_City_scale_geometry.stl", std::ifstream::in);
	if (input.is_open())
	{
	next_solid:;
		b_triangle_id = 0;//renewed for each new solid
		//Initialize an empty vectors
		std::vector<Point> polygon;
		std::vector<double> triangle_area;
		std::vector<string> triangle_name;
		std::vector<Point> triangle_vertices;
		while (input >> chr)
		{
			if (chr == "solid")
			{
				input >> chr;

			next_triangle:;
				input >> chr;

				if (chr == "facet")
				{			
					input >> chr >> num >> num >> num >> chr >> chr;
					for (int i = 0; i <= 2; i++)
					{
						input >> chr >> x[i] >> y[i] >> z[i];
					}
					//calculating and storing the area of the triangle (it is called facet in the stl file)
					Point3D p1 = { x[0], y[0], z[0] };
					Point3D p2 = { x[1], y[1], z[1] };
					Point3D p3 = { x[2], y[2], z[2] };
					
					//map vertices on the x-z plane triangle and store
					Point vertex_1 = { x[0], z[0] };
					Point vertex_2 = { x[1], z[1] };
					Point vertex_3 = { x[2], z[2] };
					//store the vertices coordinates
					triangle_vertices.push_back(vertex_1);
					triangle_vertices.push_back(vertex_2);
					triangle_vertices.push_back(vertex_3);
					//calculate and store triangle areas
					Triangle3D triangle(p1, p2, p3);
					triangle_area.push_back(triangle.calculateArea()); 
					
					//*****************************************************************//
					if (y[0] != 0 && y[1] != 0 && y[2] != 0) {
						triangle_name.push_back("roof");
					}
					else if (y[0] == 0 && y[1] == 0 && y[2] == 0) {
						triangle_name.push_back("floor");
					}
					else {
						//create the polygon if the area of the triangle is not zero
						if (triangle.calculateArea() > 0.) {
							triangle_name.push_back("envelope");
							//creating the polygon (building geometry border) based on the vertices mapped on the x-z plane
							if (b_triangle_id == 0) {
								polygon.push_back(vertex_1);
							}
							else {
								//the goal here is to avoid duplicate vertices.
								int n = polygon.size();
								bool is_vertex_1 = false;
								bool is_vertex_2 = false;
								bool is_vertex_3 = false;
								for (int k = 0; k <= n - 1; k++) {
									if (vertex_1.x == polygon[k].x && vertex_1.y == polygon[k].y) {
										is_vertex_1 = true;
									}
									else if (vertex_2.x == polygon[k].x && vertex_2.y == polygon[k].y) {
										is_vertex_2 = true;
									}
									else if (vertex_3.x == polygon[k].x && vertex_3.y == polygon[k].y) {
										is_vertex_3 = true;
									}
								}
								if (!is_vertex_1) {
									polygon.push_back(vertex_1);
								}
								if (!is_vertex_2 && (vertex_2.x!= vertex_1.x || vertex_2.y != vertex_1.y)) {
									polygon.push_back(vertex_2);
								}
								if (!is_vertex_3 && (vertex_3.x != vertex_2.x || vertex_3.y != vertex_2.y) && (vertex_3.x != vertex_1.x || vertex_3.y != vertex_1.y)) {
									polygon.push_back(vertex_3);
								}
							}							
						}
					}
					//****************************************************************//

					//create the polygon if the area of the triangle is not zero
					/*if (triangle.calculateArea() > 0. && stl_info.tri_type[j][b_triangle_id] == "envelope") {
						//creating the polygon (building geometry border) based on the vertices mapped on the x-z plane
						polygon.push_back(vertex_1);
						polygon.push_back(vertex_2);
						polygon.push_back(vertex_3);
					}*/

					input >> chr >> chr;

					//update the index of the triangle/facet within a solid building
					b_triangle_id++;

					goto next_triangle;
				}
				else if (chr == "endsolid")
				{
					int vertex_counter = 0;
					for (int k = 0; k <= b_triangle_id - 1; k++) {
						// Declare variables to finally store the coordinates of the two final points to calculate the correct direction of normal vector on each triangle
						double xLeft, yLeft, xRight, yRight;//points to determine the correct direction of the normal vector to triangles
						double xA = 0., yA = 0., xB = 0., yB = 0.;//points to calculate the azimuth angle of triangles
						//In cityBEM && stl_info.tri_type[j][b_triangle_id] == "envelope"
						if (triangle_area[k] > 0. && triangle_name[k]== "envelope") {
							//retrieve vertices
							x[0] = triangle_vertices[vertex_counter].x;
							y[0] = triangle_vertices[vertex_counter].y;
							x[1] = triangle_vertices[vertex_counter + 1].x;
							y[1] = triangle_vertices[vertex_counter + 1].y;
							x[2] = triangle_vertices[vertex_counter + 2].x;
							y[2] = triangle_vertices[vertex_counter + 2].y;
							vertex_counter += 3; // Increment vertex_counter by 3
							
							//Calculate distances of three triangle vertices mapped on the x-z plane [m]
							double dist01 = std::sqrt(std::pow(x[0] - x[1], 2) + std::pow(y[0] - y[1], 2));
							double dist02 = std::sqrt(std::pow(x[0] - x[2], 2) + std::pow(y[0] - y[2], 2));
							double dist12 = std::sqrt(std::pow(x[1] - x[2], 2) + std::pow(y[1] - y[2], 2));
							
							if (dist01 >= dist02 && dist01 >= dist12) {
								//Points 0 and 1 have the largest distance; the midpoint is between 0 and 1
								xA = (x[0] + x[1]) / 2.;
								yA = (y[0] + y[1]) / 2.;
								std::tie(xLeft, yLeft, xRight, yRight) = calculatePerpendicularPoints(x[0], y[0], x[1], y[1], 0.2);
							}
							else if (dist02 > dist01 && dist02 >= dist12) {
								//Points 0 and 2 have the largest distance; the midpoint is between 0 and 2
								xA = (x[0] + x[2]) / 2.;
								yA = (y[0] + y[2]) / 2.;
								std::tie(xLeft, yLeft, xRight, yRight) = calculatePerpendicularPoints(x[0], y[0], x[2], y[2], 0.2);
							}
							else {
								//Points 1 and 2 have the largest distance; the midpoint is between 1 and 2
								xA = (x[1] + x[2]) / 2.0;
								yA = (y[1] + y[2]) / 2.0;
								std::tie(xLeft, yLeft, xRight, yRight) = calculatePerpendicularPoints(x[1], y[1], x[2], y[2], 0.2);
							}

							//Two points on the left and right side of the perpendicular line to the triangle and 0.2 away
							Point pointA = { xLeft, yLeft };
							Point pointB = { xRight, yRight };//{ xRight, yRight };

							//Check whether pointA or pointB are inside the building footprint. When the boolean is false, it is the correct direction    
							bool resultA = isPointInPolygon(pointA, polygon);
							bool resultB = isPointInPolygon(pointB, polygon);

							if (resultA == resultB) {
								bug_found++;
								/*if ((bug_found / tri_ind) * 100 > 5.) {
									//print_error << "The bugs found in the azimuth angle function have exceeded the normal threshold (>5% of all triangles). The simulation cannot be continued" << endl;
									exit(EXIT_FAILURE);
								}*/
							}

							//Specify the coordination of the point outside the building footprint
							if (resultA == false) {
								xB = xLeft;
								yB = yLeft;
							}
							else if (resultB == false) {
								xB = xRight;
								yB = yRight;
							}
							triangle_azimuth[j][k]= calculateClockwiseAngle(xB - xA, yB - yA);
						}
						else if (triangle_area[k] > 0. && triangle_name[k] == "roof") {
							triangle_azimuth[j][k] = 0.;//the azimuth angle for a horizontal surface is typically considered as zero
						}
						else {
							//The area of the triangle is zero. Azimuth angle caanot be defined. 
							triangle_azimuth[j][k] = NAN;
						}
					}
					//Clear vectors
					polygon.clear();
					triangle_area.clear();
					triangle_vertices.clear();

					input >> chr;
					j++;//solid building counter
					goto next_solid;
				}
			}
		}
	}
	else
	{
		//print_error << "Unable to open Input_City_scale_geometry.stl file";
		exit(EXIT_FAILURE);
	}
	input.close();


    return 0;
}
