/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011-2014 University of California, Los Angeles
 *
 * See AUTHORS.md for complete list of authors and contributors.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "dummy-keychain.hpp"

#include <ndn-cxx/util/io.hpp>
#include <boost/iostreams/device/array.hpp>

namespace ndn {
namespace security {

static const uint8_t DUMMY_CERT[] = {
  0x06, 0xfd, 0x02, 0xf0, 0x07, 0x38, 0x08, 0x05, 0x64, 0x75, 0x6d, 0x6d, 0x79, 0x08, 0x03, 0x6b,
  0x65, 0x79, 0x08, 0x03, 0x4b, 0x45, 0x59, 0x08, 0x11, 0x6b, 0x73, 0x6b, 0x2d, 0x31, 0x34, 0x31,
  0x38, 0x36, 0x30, 0x30, 0x33, 0x39, 0x31, 0x30, 0x35, 0x30, 0x08, 0x07, 0x49, 0x44, 0x2d, 0x43,
  0x45, 0x52, 0x54, 0x08, 0x09, 0xfd, 0x00, 0x00, 0x01, 0x4a, 0x4b, 0x2c, 0x22, 0x5e, 0x14, 0x03,
  0x18, 0x01, 0x02, 0x15, 0xfd, 0x01, 0x73, 0x30, 0x82, 0x01, 0x6f, 0x30, 0x22, 0x18, 0x0f, 0x32,
  0x30, 0x31, 0x34, 0x31, 0x32, 0x31, 0x34, 0x32, 0x33, 0x33, 0x39, 0x35, 0x31, 0x5a, 0x18, 0x0f,
  0x32, 0x30, 0x33, 0x34, 0x31, 0x32, 0x30, 0x39, 0x32, 0x33, 0x33, 0x39, 0x35, 0x31, 0x5a, 0x30,
  0x25, 0x30, 0x23, 0x06, 0x03, 0x55, 0x04, 0x29, 0x13, 0x1c, 0x2f, 0x64, 0x75, 0x6d, 0x6d, 0x79,
  0x2f, 0x6b, 0x65, 0x79, 0x2f, 0x6b, 0x73, 0x6b, 0x2d, 0x31, 0x34, 0x31, 0x38, 0x36, 0x30, 0x30,
  0x33, 0x39, 0x31, 0x30, 0x35, 0x30, 0x30, 0x82, 0x01, 0x20, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86,
  0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0d, 0x00, 0x30, 0x82,
  0x01, 0x08, 0x02, 0x82, 0x01, 0x01, 0x00, 0xc5, 0x47, 0xe1, 0xbf, 0x9e, 0x09, 0x76, 0x07, 0xaa,
  0xd3, 0x09, 0x90, 0xfe, 0xbb, 0xbd, 0x7b, 0x0f, 0xc1, 0xc8, 0x22, 0x9c, 0x41, 0xac, 0x1a, 0x65,
  0x3f, 0x57, 0x9b, 0xda, 0x3f, 0xb0, 0xc6, 0xa1, 0xc1, 0x0f, 0x48, 0x6c, 0xdc, 0x76, 0x0f, 0x3e,
  0x09, 0x91, 0xe9, 0x5a, 0xc6, 0xb6, 0xbb, 0xf8, 0x32, 0x34, 0x40, 0xfd, 0xe9, 0x61, 0xcb, 0x6f,
  0x62, 0x49, 0xff, 0x8f, 0x0a, 0xb9, 0xcb, 0x77, 0x0e, 0xd0, 0xb2, 0x50, 0x07, 0xe9, 0x1f, 0xff,
  0xb1, 0x6d, 0x48, 0xa1, 0xbd, 0xa8, 0x11, 0x2f, 0xf1, 0xaa, 0xaa, 0xe6, 0x10, 0xd5, 0xb5, 0xe0,
  0x54, 0x2a, 0x2c, 0x94, 0x5e, 0xa0, 0xe6, 0x43, 0x36, 0xde, 0xfa, 0x64, 0xdc, 0x27, 0xd6, 0x1d,
  0x0f, 0xbb, 0x22, 0x17, 0x57, 0x61, 0xae, 0x39, 0xcc, 0xdc, 0xa2, 0xf5, 0xc6, 0x97, 0x17, 0xbb,
  0x74, 0x1a, 0x57, 0x99, 0xbf, 0x23, 0xa7, 0x0a, 0x57, 0x06, 0x2b, 0x27, 0x1a, 0x77, 0x24, 0xaa,
  0xfa, 0x93, 0x9a, 0x47, 0x87, 0x12, 0x65, 0x3c, 0x8c, 0x94, 0xbb, 0xba, 0x14, 0xde, 0x41, 0x56,
  0xed, 0xb1, 0xea, 0xda, 0xea, 0xb8, 0x79, 0xcf, 0x60, 0x05, 0x70, 0x8a, 0xc0, 0x0e, 0xb8, 0x21,
  0x56, 0x15, 0x42, 0x6d, 0x46, 0x54, 0x39, 0x3e, 0x57, 0x4c, 0x2b, 0xee, 0x9c, 0x3d, 0xa8, 0x3e,
  0x90, 0x35, 0xae, 0xde, 0x33, 0x3a, 0xc2, 0x30, 0x4e, 0xaf, 0x7c, 0xcd, 0x1d, 0x79, 0x28, 0x94,
  0x6b, 0x84, 0xe8, 0xbd, 0x42, 0x43, 0x40, 0x50, 0xec, 0x33, 0x6f, 0x3b, 0x91, 0x89, 0xb7, 0xb5,
  0x9b, 0xbb, 0x8a, 0xe2, 0x73, 0x98, 0xe0, 0xba, 0xde, 0xd9, 0x78, 0x61, 0x97, 0x72, 0xab, 0x23,
  0xa0, 0x9e, 0xa4, 0xab, 0x50, 0x61, 0x35, 0xb2, 0x83, 0x47, 0x7c, 0xba, 0x24, 0x23, 0x5d, 0xc7,
  0x85, 0x2c, 0x31, 0xa2, 0x4e, 0xe7, 0xb9, 0x02, 0x01, 0x11, 0x16, 0x34, 0x1b, 0x01, 0x01, 0x1c,
  0x2f, 0x07, 0x2d, 0x08, 0x05, 0x64, 0x75, 0x6d, 0x6d, 0x79, 0x08, 0x03, 0x6b, 0x65, 0x79, 0x08,
  0x03, 0x4b, 0x45, 0x59, 0x08, 0x11, 0x6b, 0x73, 0x6b, 0x2d, 0x31, 0x34, 0x31, 0x38, 0x36, 0x30,
  0x30, 0x33, 0x39, 0x31, 0x30, 0x35, 0x30, 0x08, 0x07, 0x49, 0x44, 0x2d, 0x43, 0x45, 0x52, 0x54,
  0x17, 0xfd, 0x01, 0x00, 0x4b, 0x2c, 0x9a, 0x10, 0xb7, 0xd1, 0xc4, 0xf7, 0x73, 0x48, 0xdc, 0xcb,
  0xf5, 0x24, 0x26, 0xf7, 0x7b, 0x30, 0x42, 0x58, 0xcc, 0x13, 0x0e, 0x76, 0x40, 0xf9, 0x74, 0x15,
  0x52, 0x05, 0x52, 0xbc, 0x84, 0x89, 0x7d, 0x4f, 0xe5, 0xa3, 0x6b, 0x27, 0x7b, 0x44, 0x9f, 0x1a,
  0x9e, 0x0a, 0xb1, 0x48, 0x63, 0xdb, 0x4e, 0x82, 0x0e, 0x9b, 0xa1, 0xde, 0xad, 0x13, 0x87, 0x98,
  0x0a, 0x48, 0x97, 0xb2, 0xb3, 0x2f, 0xdb, 0xbf, 0xae, 0x95, 0xda, 0x6d, 0x86, 0xf1, 0xa0, 0xe7,
  0x3a, 0x42, 0x7b, 0x52, 0xdf, 0xb6, 0xa3, 0xf4, 0x27, 0x16, 0x92, 0x0c, 0x27, 0x3f, 0xa8, 0x65,
  0xfd, 0x9f, 0xf5, 0x9d, 0x0f, 0x72, 0x1a, 0x1c, 0x9a, 0x4b, 0xfc, 0xcb, 0x81, 0x63, 0xd3, 0x6b,
  0x7c, 0x1c, 0x2b, 0x85, 0x3b, 0x57, 0xa4, 0x12, 0xf3, 0x9c, 0xa1, 0xc6, 0x6f, 0x59, 0xb2, 0xb2,
  0x1d, 0x7f, 0x8f, 0xd0, 0xe7, 0x5e, 0x7c, 0xfc, 0xd2, 0x0b, 0x9d, 0xf9, 0xfe, 0xbb, 0x0d, 0x1d,
  0x74, 0xf3, 0x80, 0xfb, 0x28, 0x57, 0xec, 0x9b, 0xd7, 0x7b, 0xeb, 0x9b, 0x8c, 0x0f, 0x7a, 0xa3,
  0xcf, 0x06, 0xcf, 0xac, 0x71, 0x06, 0x27, 0x78, 0x81, 0x4a, 0x91, 0x19, 0x06, 0x13, 0x93, 0xd4,
  0xa6, 0x49, 0xdc, 0x00, 0xc0, 0x4d, 0xea, 0x25, 0xa0, 0x99, 0x20, 0x36, 0x45, 0xc8, 0xdc, 0x6e,
  0x64, 0xfa, 0xba, 0xd0, 0xa3, 0x25, 0x3a, 0x96, 0xbe, 0x25, 0xf5, 0x5c, 0xf4, 0x2d, 0x1e, 0x81,
  0x7f, 0x84, 0x11, 0x82, 0xd0, 0x3f, 0xd3, 0xb5, 0x00, 0xe0, 0xc1, 0x51, 0x86, 0x89, 0xb5, 0xc1,
  0xcd, 0xbf, 0xa4, 0x95, 0x11, 0xef, 0x08, 0x2f, 0xee, 0x50, 0x87, 0xe3, 0x53, 0x61, 0xeb, 0x13,
  0xbd, 0x39, 0x91, 0xac, 0x3c, 0x6e, 0xd3, 0xe1, 0x4c, 0xfe, 0x7f, 0x3c, 0xc7, 0xbc, 0x60, 0xa9,
  0x24, 0xfa, 0x18, 0xe1
};

static const uint8_t DUMMY_SIGNATURE[] = {
  0x17, 0xfd, 0x01, 0x00, 0x93, 0x15, 0x09, 0x49, 0x79, 0x9e, 0xb7, 0x9c,
  0xd3, 0xc1, 0xbf, 0x61, 0x89, 0xd5, 0xd9, 0xca, 0xf2, 0xb0, 0x14, 0xae,
  0x72, 0x7c, 0x1f, 0x8f, 0xf5, 0xb1, 0x70, 0xd6, 0x9b, 0x8f, 0xf8, 0xd7,
  0x2d, 0xbc, 0x92, 0x6f, 0x7d, 0x77, 0x96, 0x46, 0xea, 0xd4, 0x7d, 0x90,
  0xbc, 0x7a, 0xeb, 0xe2, 0x03, 0x93, 0xb1, 0xd2, 0x62, 0xec, 0x9d, 0xff,
  0x9c, 0x9c, 0x2a, 0x14, 0x7d, 0x23, 0xca, 0x29, 0x3d, 0x15, 0x1a, 0x40,
  0x42, 0x2c, 0x59, 0x33, 0x8a, 0xf7, 0xc0, 0x6b, 0xc4, 0x9c, 0xf3, 0xc4,
  0x99, 0xa4, 0x1a, 0x60, 0xf5, 0x28, 0x7d, 0x4c, 0xef, 0x43, 0x7d, 0xbd,
  0x7d, 0x00, 0x51, 0xee, 0x41, 0xf5, 0x25, 0x80, 0xce, 0xe6, 0x64, 0x4f,
  0x75, 0x54, 0xf3, 0xb2, 0x99, 0x9a, 0x0f, 0x93, 0x9a, 0x28, 0x1d, 0xfe,
  0x12, 0x8a, 0xe0, 0xc1, 0x02, 0xeb, 0xa4, 0x35, 0x52, 0x88, 0xac, 0x44,
  0x1a, 0x44, 0x82, 0x97, 0x4f, 0x5f, 0xa8, 0xd8, 0x9f, 0x67, 0x38, 0xa8,
  0x64, 0xb6, 0x62, 0x99, 0xbd, 0x96, 0x3c, 0xf5, 0x86, 0x09, 0x5c, 0x97,
  0x6b, 0x8f, 0xae, 0xe0, 0x60, 0xe7, 0x23, 0x98, 0x6a, 0xee, 0xc1, 0xb0,
  0x14, 0xbe, 0x46, 0x2c, 0xfb, 0xa7, 0x27, 0x73, 0xe4, 0xf3, 0x26, 0x33,
  0xba, 0x99, 0xd4, 0x01, 0x38, 0xa8, 0xf2, 0x9e, 0x87, 0xe0, 0x71, 0x0b,
  0x25, 0x44, 0x07, 0x35, 0x88, 0xab, 0x67, 0x27, 0x56, 0x0e, 0xb5, 0xb5,
  0xe8, 0x27, 0xb4, 0x49, 0xdc, 0xb8, 0x48, 0x31, 0xff, 0x99, 0x48, 0xab,
  0x11, 0xb4, 0xa0, 0xdf, 0x8a, 0x6d, 0xff, 0x43, 0x69, 0x32, 0xa7, 0xbc,
  0x63, 0x9d, 0x0f, 0xe0, 0x95, 0x34, 0x36, 0x25, 0x4b, 0x3e, 0x36, 0xbd,
  0x81, 0x91, 0x0b, 0x91, 0x9f, 0x3a, 0x04, 0xa2, 0x44, 0x28, 0x19, 0xa1,
  0x38, 0x21, 0x4f, 0x25, 0x59, 0x8a, 0x48, 0xc2
};

bool
DummyPublicInfo::doesIdentityExist(const Name& identityName)
{
  return true;
}

void
DummyPublicInfo::addIdentity(const Name& identityName)
{
}

bool
DummyPublicInfo::revokeIdentity()
{
  return true;
}

bool
DummyPublicInfo::doesPublicKeyExist(const Name& keyName)
{
  return true;
}

void
DummyPublicInfo::addKey(const Name& keyName, const PublicKey& publicKey)
{
}

shared_ptr<PublicKey>
DummyPublicInfo::getPublicKey(const Name& keyName)
{
  static shared_ptr<PublicKey> publicKey = nullptr;
  if (publicKey == nullptr) {
    typedef boost::iostreams::stream<boost::iostreams::array_source> arrayStream;
    arrayStream is(reinterpret_cast<const char*>(DUMMY_CERT), sizeof(DUMMY_CERT));
    auto cert = io::load<IdentityCertificate>(is, io::NO_ENCODING);
    publicKey = make_shared<PublicKey>(cert->getPublicKeyInfo());
  }

  return publicKey;
}

KeyType
DummyPublicInfo::getPublicKeyType(const Name& keyName)
{
  return KEY_TYPE_RSA;
}

bool
DummyPublicInfo::doesCertificateExist(const Name& certificateName)
{
  return true;
}

void
DummyPublicInfo::addCertificate(const IdentityCertificate& certificate)
{
}

shared_ptr<IdentityCertificate>
DummyPublicInfo::getCertificate(const Name& certificateName)
{
  static shared_ptr<IdentityCertificate> cert = nullptr;
  if (cert == nullptr) {
    typedef boost::iostreams::stream<boost::iostreams::array_source> arrayStream;
    arrayStream is(reinterpret_cast<const char*>(DUMMY_CERT), sizeof(DUMMY_CERT));
    cert = io::load<IdentityCertificate>(is, io::NO_ENCODING);
  }

  return cert;
}

Name
DummyPublicInfo::getDefaultIdentity()
{
  return "/dummy/key";
}

Name
DummyPublicInfo::getDefaultKeyNameForIdentity(const Name& identityName)
{
  return "/dummy/key/ksk-1418600391050";
}

Name
DummyPublicInfo::getDefaultCertificateNameForKey(const Name& keyName)
{
  return "/dummy/key/KEY/ksk-1418600391050/ID-CERT/%FD%00%00%01JK%2C%22%5E";
}

void
DummyPublicInfo::getAllIdentities(std::vector<Name>& nameList, bool isDefault)
{
  if (isDefault) {
    nameList.push_back("/dummy");
  }
}

void
DummyPublicInfo::getAllKeyNames(std::vector<Name>& nameList, bool isDefault)
{
  if (isDefault) {
    nameList.push_back("/dummy/key/ksk-1418600391050");
  }
}

void
DummyPublicInfo::getAllKeyNamesOfIdentity(const Name& identity,
                                          std::vector<Name>& nameList, bool isDefault)
{
  if (isDefault) {
    nameList.push_back("/dummy/key/ksk-1418600391050");
  }
}

void
DummyPublicInfo::getAllCertificateNames(std::vector<Name>& nameList, bool isDefault)
{
  if (isDefault) {
    nameList.push_back("/dummy/key/KEY/ksk-1418600391050/ID-CERT/%FD%00%00%01JK%2C%22%5E");
  }
}

void
DummyPublicInfo::getAllCertificateNamesOfKey(const Name& keyName,
                                             std::vector<Name>& nameList, bool isDefault)
{
  if (isDefault) {
    nameList.push_back("/dummy/key/KEY/ksk-1418600391050/ID-CERT/%FD%00%00%01JK%2C%22%5E");
  }
}

void
DummyPublicInfo::deleteCertificateInfo(const Name& certificateName)
{
}

void
DummyPublicInfo::deletePublicKeyInfo(const Name& keyName)
{
}

void
DummyPublicInfo::deleteIdentityInfo(const Name& identity)
{
}

void
DummyPublicInfo::setDefaultIdentityInternal(const Name& identityName)
{
}

void
DummyPublicInfo::setDefaultKeyNameForIdentityInternal(const Name& keyName)
{
}

void
DummyPublicInfo::setDefaultCertificateNameForKeyInternal(const Name& certificateName)
{
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void
DummyTpm::setTpmPassword(const uint8_t* password, size_t passwordLength)
{
}

void
DummyTpm::resetTpmPassword()
{
}

void
DummyTpm::setInTerminal(bool inTerminal)
{
}

bool
DummyTpm::getInTerminal() const
{
  return false;
}

bool
DummyTpm::isLocked()
{
  return false;
}

bool
DummyTpm::unlockTpm(const char* password, size_t passwordLength, bool usePassword)
{
  return true;
}

void
DummyTpm::generateKeyPairInTpm(const Name& keyName, const KeyParams& params)
{
}

void
DummyTpm::deleteKeyPairInTpm(const Name& keyName)
{
}

shared_ptr<PublicKey>
DummyTpm::getPublicKeyFromTpm(const Name& keyName)
{
  return nullptr;
}

Block
DummyTpm::signInTpm(const uint8_t* data, size_t dataLength,
                    const Name& keyName,
                    DigestAlgorithm digestAlgorithm)
{
  return Block(DUMMY_SIGNATURE, sizeof(DUMMY_SIGNATURE));
}

ConstBufferPtr
DummyTpm::decryptInTpm(const uint8_t* data, size_t dataLength,
                       const Name& keyName, bool isSymmetric)
{
  throw Error("Not supported");
}

ConstBufferPtr
DummyTpm::encryptInTpm(const uint8_t* data, size_t dataLength,
                       const Name& keyName, bool isSymmetric)
{
  throw Error("Not supported");
}

void
DummyTpm::generateSymmetricKeyInTpm(const Name& keyName, const KeyParams& params)
{
}

bool
DummyTpm::doesKeyExistInTpm(const Name& keyName, KeyClass keyClass)
{
  return true;
}

bool
DummyTpm::generateRandomBlock(uint8_t* res, size_t size)
{
  return false;
}

void
DummyTpm::addAppToAcl(const Name& keyName, KeyClass keyClass,
                      const std::string& appPath, AclType acl)
{
}

ConstBufferPtr
DummyTpm::exportPrivateKeyPkcs8FromTpm(const Name& keyName)
{
  throw Error("Not supported");
}

bool
DummyTpm::importPrivateKeyPkcs8IntoTpm(const Name& keyName,
                                       const uint8_t* buffer, size_t bufferSize)
{
  return false;
}

bool
DummyTpm::importPublicKeyPkcs1IntoTpm(const Name& keyName,
                                      const uint8_t* buffer, size_t bufferSize)
{
  return false;
}


} // namespace security
} // namespace ndn