//
//  main.cpp
//  Lab2
//
//  Copyright © 2016 CGIS. All rights reserved.
//

#include <iostream>
#include "testMatrix.h"
#include "testTransform.h"
#include "testVector.h"

int main(int argc, const char *argv[]) {

    int nrOfErrors = 0;

    nrOfErrors += ecg::testVec2Implementation();
    nrOfErrors += ecg::testVec3Implementation();
    nrOfErrors += ecg::testVec4Implementation();
    nrOfErrors += ecg::testMat3Implementation();
    nrOfErrors += ecg::testMat4Implementation();
    nrOfErrors += ecg::test2DTransformImplementation();
    nrOfErrors += ecg::test3DTransformImplementation();

    std::cout << "Number of errors: " << nrOfErrors << std::endl;

    std::getchar();

    return 0;
}
