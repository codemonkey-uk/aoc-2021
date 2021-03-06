// twentytwo.cpp
// Day 22: Reactor Reboot

#include "geometry/aabb3d.h"
#include "geometry/aabb_fn.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <limits>

using namespace std;
using namespace Geometry;

extern bool debug;

typedef AxisAlignedBoundingBox3d<long long> Aabb;

typedef pair< Aabb, int > Command;

Command ReadCommand(const string& line)
{
    int x1=0,x2=0,y1=0,y2=0,z1=0,z2=0,onoff=-1;
    
    // on
    if (line.rfind("on",0)==0)
    {
        onoff=1;
        sscanf(
            line.c_str(), 
            "on x=%i..%i,y=%i..%i,z=%i..%i",
            &x1,&x2,&y1,&y2,&z1,&z2
        );
    }
    else if (line.rfind("off",0)==0)
    {
        onoff=0;
        sscanf(
            line.c_str(), 
            "off x=%i..%i,y=%i..%i,z=%i..%i",
            &x1,&x2,&y1,&y2,&z1,&z2
        );
    }
    
    if (x1>x2) swap(x1,x2);
    if (y1>y2) swap(y1,y2);
    if (z1>z2) swap(z1,z2);
    
    Command result{{{x1,y1,z1},{x2+1,y2+1,z2+1}},onoff};    
    return result;
}

size_t DebugStat(const vector< Aabb >& onBoxes)
{
    if (debug) cout << onBoxes.size() << " boxes ";
    size_t total = 0;
    for(Aabb a : onBoxes )
        total += a.GetVolume();
    if (debug) cout << total << " cubes." << endl;
    return total;
}

void TwentyTwo()
{
    string line;

    vector< Aabb > onBoxes;
    size_t runningTotal = 0;

    //x=-50..50,y=-50..50,z=-50..50
    // Aabb clipRegion({-50,-50,-50},{50+1,50+1,50+1});
    Aabb clipRegion({
        numeric_limits<Aabb::VectorType::ScalarType>::lowest(),
        numeric_limits<Aabb::VectorType::ScalarType>::lowest(),
        numeric_limits<Aabb::VectorType::ScalarType>::lowest()
    },
    {
        numeric_limits<Aabb::VectorType::ScalarType>::max(),
        numeric_limits<Aabb::VectorType::ScalarType>::max(),
        numeric_limits<Aabb::VectorType::ScalarType>::max()
    });
    
    while (getline(cin, line))
    {
        Command c = ReadCommand(line);
        if (debug) cout << line << endl;
        if (clipRegion.Contains(c.first))
        {
            if (c.second==1)
            {
                const auto max_diff = c.first.GetVolume();
                if (debug)
                {
                    cout << "turn ON a " 
                        << c.first.GetAxisExtent(0) << "x"
                        << c.first.GetAxisExtent(1) << "x"
                        << c.first.GetAxisExtent(2) << " cuboid " 
                        << " (" << max_diff << " cubes)" 
                        << endl;
                }
                vector< Aabb > newBoxes;
                newBoxes.push_back( c.first );
                for (Aabb a : onBoxes)
                {
                    vector< Aabb > remainingBoxes;
                    auto itr = back_inserter(remainingBoxes);
                    for (Aabb b : newBoxes)
                    {
                        AABB_Difference(a, b, itr);
                    }
                    newBoxes = remainingBoxes;
                }
                onBoxes.insert( onBoxes.end(), newBoxes.begin(), newBoxes.end() );
                size_t newTotal = DebugStat(onBoxes);
                const int diff = (newTotal - runningTotal);
                if (debug) cout << " = " << diff << " additional cubes" << endl;
                assert(diff<=max_diff);
                runningTotal = newTotal;
            }
            else if (c.second==0)
            {
                const auto max_diff = c.first.GetVolume();
                if (debug)
                {
                    cout << "turn OFF a " 
                        << c.first.GetAxisExtent(0) << "x"
                        << c.first.GetAxisExtent(1) << "x"
                        << c.first.GetAxisExtent(2) << " cuboid " 
                        << " (" << max_diff << " cubes)" 
                        << endl;
                }
                vector< Aabb > remainingBoxes;
                auto itr = back_inserter(remainingBoxes);
                for (Aabb a : onBoxes)
                {
                    // everything from A *not* (in our list) covered by C 
                    AABB_Difference(c.first, a, itr);
                }

                size_t newTotal = DebugStat(remainingBoxes);
                const int diff = runningTotal-newTotal;
                if (debug) cout << " = " << diff << " cubes removed" << endl;
                assert(diff<=max_diff);
                runningTotal = newTotal;
                onBoxes = remainingBoxes;
            }
            else 
            {
                cerr << "Unexpected command type in line: " << line << endl;
            }
        }
    }
    
    cout << "Result: " << runningTotal << " cubes are on ("
        << onBoxes.size() << " boxes)"<< endl;
}

