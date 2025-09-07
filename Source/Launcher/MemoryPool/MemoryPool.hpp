#include <string>

namespace Simple
{
	class MemoryPool final
	{
	public:
		MemoryPool(const size_t aTypeSize, const size_t aTypeAlignment, const size_t aTypeHashCode, const std::string& aComponentName, const size_t aDefaultMemoryReserveSize = 8);
		~MemoryPool();
	private:
		char* myCurrentMemoryAddress;
		char* myStartMemoryAddress;
		char* myEndMemoryAddress;

		std::string myName;
		size_t myTypeHashCode;
		size_t myTypeAlignment;
	};
}