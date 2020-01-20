
#include "VoteContent.h"
#include "../Utils.h"

void VoteContent::Serialize(ByteStream& ostream)
{
    ostream.writeBytes(&mVoteType, 1);
    ostream.putVarUint(mCandidates.size());
    if (mVoteType == 0) {
        for (auto item : mCandidates) {
            CMBlock pubKey = Utils::decodeHex(item->mCandidate);
            ostream.putVarUint(pubKey.GetSize());
            ostream.writeBytes(pubKey, pubKey.GetSize());
            ostream.writeUint64(item->mAmount);
        }
    }
    else {
        for (auto item : mCandidates){
            UInt168 u;
            bool r = Utils::UInt168FromAddress(u, item->mCandidate);
            ostream.putVarUint(sizeof(u));
            ostream.writeBytes(u.u8, sizeof(u));
            ostream.writeUint64(item->mAmount);
        }
    }
}

uint8_t VoteContent::GetVoteType()
{
    return mVoteType;
}

void VoteContent::FromJson(const std::vector<nlohmann::json>& array)
{
    for (auto item : array) {
        std::string candidate = item["candidate"];
        uint64_t amount = item["value"];
        mCandidates.push_back(std::make_shared<Candidate>(candidate, amount));
    }
}

std::vector<nlohmann::json> VoteContent::ToJson()
{
    std::vector<nlohmann::json> jsons;
    for (auto item : mCandidates) {
        nlohmann::json json;
        json["candidate"] = item->mCandidate;
        json["value"] = item->mAmount;
        jsons.push_back(json);
    }
    return jsons;
}
