#include <vuelib>

void buttonOnClicked(VMLHandle* VMLPtr) {
    /*
     * 只需要 C++ 端改变 pushCount 变量，
     * conter-text-label 的文字就会改变
    */
    auto pushCount = VMLPtr->GetVariable(L"push-count");

    ++pushCount;
}

int main() {
    VApplication Applicaiton;
    VMLWidget    Widget(L"./newvml.xml", &Applicaiton);

    Widget.AddMetaFunction(VMetaFunction(buttonOnClicked));

    return Applicaiton.exec();
}