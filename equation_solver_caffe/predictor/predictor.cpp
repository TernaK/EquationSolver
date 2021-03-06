#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace cv::dnn;

#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

/* Find best class for the blob (i. e. class with maximal probability) */
void getMaxClass(dnn::Blob &probBlob, int *classId, double *classProb)
{
    Mat probMat = probBlob.matRefConst().reshape(1, 1); //reshape the blob to 1x1000 matrix
    Point classNumber;
    std::cout << probMat << endl;
    minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
    *classId = classNumber.x;
}

std::vector<String> readClassNames(const char *filename = "synset_words.txt")
{
    std::vector<String> classNames;

    std::ifstream fp(filename);
    if (!fp.is_open())
    {
        std::cerr << "File with classes labels not found: " << filename << std::endl;
        exit(-1);
    }

    std::string name;
    while (!fp.eof())
    {
        std::getline(fp, name);
        if (name.length())
            classNames.push_back( name.substr(name.find(' ')+1) );
    }

    fp.close();
    return classNames;
}

int main(int argc, char **argv)
{
    String modelTxt = argv[1]; 
    String modelBin = argv[2];
    String imageFile = argv[3];

    if(argc != 4){
      std::cerr << "predictor <modelTxt> <modelBin> <imageFile>" << std::endl;
      exit(-1);
    }

    //! [Create the importer of Caffe model]
    Ptr<dnn::Importer> importer;
    try                                     //Try to import Caffe GoogleNet model
    {
        importer = dnn::createCaffeImporter(modelTxt, modelBin);
    }
    catch (const cv::Exception &err)        //Importer can throw errors, we will catch them
    {
        std::cerr << err.msg << std::endl;
    }
    //! [Create the importer of Caffe model]

    if (!importer)
    {
        std::cerr << "Can't load network by using the following files: " << std::endl;
        std::cerr << "prototxt:   " << modelTxt << std::endl;
        std::cerr << "caffemodel: " << modelBin << std::endl;
        exit(-1);
    }
  
    //! [Initialize network]
    dnn::Net net;
    importer->populateNet(net);
    importer.release();                     //We don't need importer anymore
    //! [Initialize network]

    //! [Prepare blob]
    Mat img = imread(imageFile, IMREAD_GRAYSCALE);
    if (img.empty())
    {
        std::cerr << "Can't read image from the file: " << imageFile << std::endl;
        exit(-1);
    }
   
    img.convertTo(img, CV_32F);
    img /= 255.0f;
    //resize(img, img, Size(28, 28));       //GoogLeNet accepts only 224x224 RGB-images
    dnn::Blob inputBlob = dnn::Blob::fromImages(img);   //Convert Mat to dnn::Blob image batch
    //! [Prepare blob]

    //! [Set input blob]
    net.setBlob(".data", inputBlob);        //set the network input
    //! [Set input blob]

    //! [Make forward pass]
    net.forward();                          //compute output
    //! [Make forward pass]

    //! [Gather output]
    dnn::Blob prob = net.getBlob("prob");   //gather output of "prob" layer

    int classId;
    double classProb;
    getMaxClass(prob, &classId, &classProb);//find the best class
    //! [Gather output]

    //! [Print results]
    // std::vector<String> classNames = readClassNames();
    std::cout << "Best class: #" << classId << std::endl; //<< " '" << classNames.at(classId) << "'" << std::endl;
    //std::cout << "Probability: " << classProb * 100 << "%" << std::endl;
    //! [Print results]

    return 0;
} //main
