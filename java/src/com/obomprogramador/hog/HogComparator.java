package com.obomprogramador.hog;
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
*/
import static org.opencv.imgcodecs.Imgcodecs.imread;

import org.opencv.core.Mat;

/**
	This is a simple faces comparator using HOG (dblib).
	I recommend that you transform this Java code into a RESTful service.
 */
public class HogComparator {

    static {
    	nu.pattern.OpenCV.loadShared();
        System.loadLibrary("hogcomparator");
    }

	// Native method implemented by a C++ library:
	private native boolean compareFaces(long addressPhoto1, long addressPhoto2);
	
	public static void main(String[] args) {
		if (args.length < 2) {
			System.out.println("Missing arguments photo 1 and/or photo 2");
		}
		else {
			try {
				Mat photo1 = imread(args[0]);
				Mat photo2 = imread(args[1]);
				HogComparator hg = new HogComparator();
				System.out.println("Images are from the same person? " 
					+ hg.compareFaces(photo1.getNativeObjAddr(),photo2.getNativeObjAddr()));
			}
			catch (Exception ex) {
				System.out.println("Something wrong with these files: " + ex.getMessage());
			}
		}
	}
}
