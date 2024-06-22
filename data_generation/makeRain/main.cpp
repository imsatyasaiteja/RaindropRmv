#include <iostream>
#include <random>
#include <glob.h>
#include <regex>
#include <boost/format.hpp>

#include "rain.h"

using namespace std;
using boost::format;
using boost::str;

void getFiles(const string &pattern, vector<string> &filePath);

int main()
{
    map<string, double> params;
    vector<string> imgPath;
    getFiles("/home/teja/Documents/DIP/Grp2/Assgn_1/code1/rootA/*.png", imgPath);

    int totalIndex = imgPath.size(), numsPerImg{1};
    cout << totalIndex << endl;

    mt19937 rng;
    rng.seed(random_device()());
    uniform_int_distribution<int> random_M(100, 200);
    uniform_int_distribution<int> random_B(4000, 8000);
    uniform_int_distribution<int> random_psi(60, 90);
    uniform_int_distribution<int> random_dia(4, 8);

    string savePath{"/home/teja/Documents/DIP/Grp2/Assgn_1/code1/Synthesize_code1/"};

    for (int index{0}; index < totalIndex; ++index)
    {
        if (index % 10 == 0)
        {
            cout << "Processing: " << setprecision(2) << index << " / " << totalIndex << " (" << float(index) / totalIndex << ")" << endl;
        }
        unsigned count{0};

        params["M"] = 100;
        params["B"] = 8000;
        params["psi"] = random_psi(rng);
        Rain rain(params, imgPath[index]);

        cv::Mat img;

        cv::resize(rain.image, img, cv::Size(), 1.5, 1.5);
        cv::imwrite(str(format("%1%/%2%_Original.png") % savePath % index), img);

        rain.render();
        cv::Mat rain_img;
        cv::resize(rain.rain_image, rain_img, cv::Size(), 1.5, 1.5);
        //cv::imwrite(str(format("%1%/%2%_Rendered.png") % savePath % index), rain_img);

        auto kernel = rain.get_kernel(random_dia(rng));

        rain.blur(kernel);
        cv::Mat mask, blur;
        cv::resize(rain.mask, mask, cv::Size(), 1.5, 1.5, cv::INTER_NEAREST);
        cv::resize(rain.blur_image, blur, cv::Size(), 1.5, 1.5);

        cv::imwrite(str(format("%1%/%2%_Final.png") % savePath % index), rain.blur_image);
        cv::imwrite(str(format("%1%/%2%_Mask.png") % savePath % index), rain.mask);
    }
    return 0;
}

void getFiles(const string &pattern, vector<string> &filePath)
{
    glob_t globBuf;
    glob(pattern.c_str(), GLOB_TILDE, NULL, &globBuf);

    for (unsigned i{0}; i < globBuf.gl_pathc; i++)
    {
        filePath.push_back(globBuf.gl_pathv[i]);
    }

    if (globBuf.gl_pathc > 0)
    {
        globfree(&globBuf);
    }
}
