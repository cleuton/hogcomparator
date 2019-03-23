/*
Copyright 2017 Cleuton Sampaio de Melo Junior - O Bom Programador
http://obomprogramador.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To run this lib you will need two files: 
- shape_predictor_5_face_landmarks.dat
- dlib_face_recognition_resnet_model_v1.dat

You can get them from: 
- "http://dlib.net/files/shape_predictor_5_face_landmarks.dat.bz2"
- "http://dlib.net/files/dlib_face_recognition_resnet_model_v1.dat.bz2"

And you must set an environment variable with the path where you extract them:
- export HOGCOMPARATOR_PATH="<path>"

THIS CODE IS BASED ON 
http://dlib.net/dnn_face_recognition_ex.cpp.html

*/
#include <jni.h>       
#include <iostream>    
#include <cstdlib>
#include "com_obomprogramador_hog_HogComparator.h"   

//------------------------------------------------------------------
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <dlib/image_processing.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/dnn.h>
#include <dlib/gui_widgets.h>
#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>

using namespace dlib;
using namespace std;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N,BN,1,tag1<SUBNET>>>;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2,2,2,2,skip1<tag2<block<N,BN,2,tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET> 
using block  = BN<con<N,3,3,1,1,relu<BN<con<N,3,3,stride,stride,SUBNET>>>>>;

template <int N, typename SUBNET> using ares      = relu<residual<block,N,affine,SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block,N,affine,SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256,SUBNET>;
template <typename SUBNET> using alevel1 = ares<256,ares<256,ares_down<256,SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128,ares<128,ares_down<128,SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64,ares<64,ares<64,ares_down<64,SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32,ares<32,ares<32,SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128,avg_pool_everything<
                            alevel0<
                            alevel1<
                            alevel2<
                            alevel3<
                            alevel4<
                            max_pool<3,3,2,2,relu<affine<con<32,7,7,2,2,
                            input_rgb_image_sized<150>
                            >>>>>>>>>>>>;

// ----------------------------------------------------------------------------------------

std::vector<matrix<rgb_pixel>> jitter_image(
    const matrix<rgb_pixel>& img
);

//------------------------------------------------------------------


typedef unsigned char BYTE;
/*
    This is the native method. 
    You should avoid these prints (cout). Use exceptions to signal errors!
*/
JNIEXPORT jboolean JNICALL Java_com_obomprogramador_hog_HogComparator_compareFaces
    (JNIEnv * env, jobject obj, jlong addFoto1, jlong addFoto2) {
        const char* pPath = getenv ("HOGCOMPARATOR_PATH");
        std::string path(pPath);
        cv::Mat* pInputImage = (cv::Mat*)addFoto1;
        cv::Mat* pInputImage2 = (cv::Mat*)addFoto2;
        dlib::array2d<rgb_pixel> dlibImage;
        dlib::array2d<rgb_pixel> dlibImage2;        
        dlib::assign_image(dlibImage, dlib::cv_image<bgr_pixel>(*pInputImage));
        dlib::assign_image(dlibImage2, dlib::cv_image<bgr_pixel>(*pInputImage2));
        dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
        dlib::shape_predictor sp;
        dlib::deserialize(path + "/shape_predictor_5_face_landmarks.dat") >> sp;
        anet_type net;
        dlib::deserialize(path + "/dlib_face_recognition_resnet_model_v1.dat") >> net;
        std::vector<matrix<rgb_pixel>> faces1;
        for (auto face : detector(dlibImage))
        {
            auto shape = sp(dlibImage, face);
            matrix<rgb_pixel> face_chip;
            dlib::extract_image_chip(dlibImage, dlib::get_face_chip_details(shape,150,0.25), face_chip);
            faces1.push_back(move(face_chip));
            break;
        }

        // Check whether there is any face in image 1:
        if (faces1.size() == 0)
        {
            cout << "No faces found in image 1!" << endl;
            return false;
        }

        std::vector<matrix<rgb_pixel>> faces2;
        for (auto face : detector(dlibImage2))
        {
            auto shape = sp(dlibImage2, face);
            matrix<rgb_pixel> face_chip;
            dlib::extract_image_chip(dlibImage2, dlib::get_face_chip_details(shape,150,0.25), face_chip);
            faces2.push_back(move(face_chip));
            break;
        }

        // Check whether there is any face in image 2:
        if (faces2.size() == 0)
        {
            cout << "No faces found in image 2!" << endl;
            return false;
        }

        std::vector<matrix<float,0,1>> face_descriptors1 = net(faces1);
        std::vector<matrix<float,0,1>> face_descriptors2 = net(faces2);

        bool thereIsAmatch = false; 
        for (size_t i = 0; i < face_descriptors1.size(); ++i)
        {
            for (size_t j = i; j < face_descriptors2.size(); ++j)
            {
                if (length(face_descriptors1[i]-face_descriptors2[j]) < 0.6)
                    thereIsAmatch = true;
            }
        }

        return thereIsAmatch;
}

//int main (int argc, char *argv[]) {}
