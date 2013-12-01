#ifndef IPATCHES_H
#define IPATCHES_H

#include "IPatch.h"
#include "Types.h"

EDB_EXPORT class IPatches {
public:
   EDB_EXPORT virtual IPatch::pointer create_patch(edb::address_t address, const void *orgBuf, const void *buf, std::size_t len) const = 0;
};
#endif // IPATCHES_H
