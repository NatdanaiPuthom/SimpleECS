
namespace Simple
{
	class MemoryPool final
	{
	public:
		MemoryPool(const size_t aTypeSize, const size_t aTypeAlignment, const size_t aTypeHashCode, const size_t aReserveAmount = 8);
		~MemoryPool();
	private:
		char* myCurrentMemoryAddress;
		char* myStartMemoryAddress;
		char* myEndMemoryAddress;

		size_t myTypeHashCode;
		size_t myTypeAlignment;
	};
}