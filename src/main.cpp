#include <blockchain.h>
#include <ecdsa.h>

using namespace encoin;

int main()
{    
    encoin::key_t kk = KEY("0C28FCA386C7A227600B2FE50B7CAE_SAMPLE_PRIVATE_KEY_DO_NOT_IMPORT_11EC86D3BF1FBE471BE89827E19D72AA1D");
    encoin::ec_point pair(kk);
    std::cout << pair.public_key() << std::endl;
    return 0;
}
