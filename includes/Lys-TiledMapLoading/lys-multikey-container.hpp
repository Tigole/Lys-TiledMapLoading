#ifndef _LYS_MULTIKEY_CONTAINER_HPP
#define _LYS_MULTIKEY_CONTAINER_HPP 1

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace lys
{

template<typename ElementType>
class MultiKeyContainer
{
public:
    void AddElement(const ElementType& element, uint32_t element_id, const std::string& element_name);
    void Clear(void);

    const ElementType* GetElementByName(const std::string& element_name) const;
    const ElementType* GetElementById(uint32_t element_id) const;

    bool IterateOverElements(std::function<bool(const ElementType&)> callback) const;

private:
    std::vector<ElementType> elements {};
    std::map<uint32_t, std::size_t> elementsIdMap {};
    std::map<std::string, std::size_t> elementsNameMap {};
};

}  // namespace lys

#endif  // _LYS_MULTIKEY_CONTAINER_HPP
