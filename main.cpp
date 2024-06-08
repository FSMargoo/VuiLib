#include <iostream>
#include <vtest/vException.h>

void B() {
	printf("A");
}

void C() {
	Core::VSignal<> A;
	A.Connect(B);
	A.Connect(B);
	A.Connect(B);
	A.Connect(B);
	A.Connect(B);
	A.Emit();
}

int main() {
	std::function<void()> Function;
	Function = B;
	std::function<void()> Function1;
	Function1 = C;
	auto _uuid = reinterpret_cast<long long>(Function.template target<void>());
	auto _uuid1 = reinterpret_cast<long long>(Function1.template target<void>());

	printf("%lld\n%lld", _uuid, _uuid1);

    return 0;
}
