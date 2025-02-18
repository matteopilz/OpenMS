// Copyright (c) 2002-present, The OpenMS Team -- EKU Tuebingen, ETH Zurich, and FU Berlin
// SPDX-License-Identifier: BSD-3-Clause
//
// --------------------------------------------------------------------------
// $Maintainer: Timo Sachsenberg $
// $Authors: Stephan Aiche $
// --------------------------------------------------------------------------

#include <OpenMS/ANALYSIS/QUANTITATION/TMTSixteenPlexQuantitationMethod.h>

#include <OpenMS/DATASTRUCTURES/ListUtils.h>
#include <OpenMS/DATASTRUCTURES/Matrix.h>


#include <algorithm>


namespace OpenMS
{
const String TMTSixteenPlexQuantitationMethod::name_ = "tmt16plex";
const std::vector<std::string> TMTSixteenPlexQuantitationMethod::channel_names_ = {"126","127N","127C","128N","128C","129N","129C","130N","130C","131N","131C","132N","132C","133N","133C","134N"};

TMTSixteenPlexQuantitationMethod::TMTSixteenPlexQuantitationMethod()
{
    setName("TMTSixteenPlexQuantitationMethod");

    // create the channel map
    channels_.push_back(IsobaricChannelInformation("126",   0, "", 126.127726, {-1, -1, -1, -1,    1,  2,  3,  4}));
    channels_.push_back(IsobaricChannelInformation("127N",  1, "", 127.124761, {-1, -1, -1,  0,   -1,  3, -1,  5}));
    channels_.push_back(IsobaricChannelInformation("127C",  2, "", 127.131081, {-1, -1,  0, -1,    3,  4,  5,  6}));
    channels_.push_back(IsobaricChannelInformation("128N",  3, "", 128.128116, {-1,  0,  1,  2,   -1,  5, -1,  7}));
    channels_.push_back(IsobaricChannelInformation("128C",  4, "", 128.134436, { 0, -1,  2, -1,    5,  6,  7,  8}));
    channels_.push_back(IsobaricChannelInformation("129N",  5, "", 129.131471, { 1,  2,  3,  4,   -1,  7, -1,  9}));
    channels_.push_back(IsobaricChannelInformation("129C",  6, "", 129.137790, { 2, -1,  4, -1,    7,  8,  9, 10}));
    channels_.push_back(IsobaricChannelInformation("130N",  7, "", 130.134825, { 3,  4,  5,  6,   -1,  9, -1, 11}));
    channels_.push_back(IsobaricChannelInformation("130C",  8, "", 130.141145, { 4, -1,  6, -1,    9, 10, 11, 12}));
    channels_.push_back(IsobaricChannelInformation("131N",  9, "", 131.138180, { 5,  6,  7,  8,   -1, 11, -1, 13}));
    channels_.push_back(IsobaricChannelInformation("131C", 10, "", 131.144500, { 6, -1,  8, -1,   11, 12, 13, 14}));
    channels_.push_back(IsobaricChannelInformation("132N", 11, "", 132.141535, { 7,  8,  9, 10,   -1, 13, -1, 15}));
    channels_.push_back(IsobaricChannelInformation("132C", 12, "", 132.147855, { 8, -1, 10, -1,   13, 14, 15, -1}));
    channels_.push_back(IsobaricChannelInformation("133N", 13, "", 133.144890, { 9, 10, 11, 12,   -1, 15, -1, -1}));
    channels_.push_back(IsobaricChannelInformation("133C", 14, "", 133.151210, {10, -1, 12, -1,   15, -1, -1, -1}));
    channels_.push_back(IsobaricChannelInformation("134N", 15, "", 134.148245, {11, 12, 13, 14,   -1, -1, -1, -1}));

    // we assume 126 to be the reference
    reference_channel_ = 0;

    setDefaultParams_();
}

void TMTSixteenPlexQuantitationMethod::setDefaultParams_()
{
    defaults_.setValue("channel_126_description", "", "Description for the content of the 126 channel.");
    defaults_.setValue("channel_127N_description", "", "Description for the content of the 127N channel.");
    defaults_.setValue("channel_127C_description", "", "Description for the content of the 127C channel.");
    defaults_.setValue("channel_128N_description", "", "Description for the content of the 128N channel.");
    defaults_.setValue("channel_128C_description", "", "Description for the content of the 128C channel.");
    defaults_.setValue("channel_129N_description", "", "Description for the content of the 129N channel.");
    defaults_.setValue("channel_129C_description", "", "Description for the content of the 129C channel.");
    defaults_.setValue("channel_130N_description", "", "Description for the content of the 130N channel.");
    defaults_.setValue("channel_130C_description", "", "Description for the content of the 130C channel.");
    defaults_.setValue("channel_131N_description", "", "Description for the content of the 131N channel.");
    defaults_.setValue("channel_131C_description", "", "Description for the content of the 131C channel.");
    defaults_.setValue("channel_132N_description", "", "Description for the content of the 132N channel.");
    defaults_.setValue("channel_132C_description", "", "Description for the content of the 132C channel.");
    defaults_.setValue("channel_133N_description", "", "Description for the content of the 133N channel.");
    defaults_.setValue("channel_133C_description", "", "Description for the content of the 133C channel.");
    defaults_.setValue("channel_134N_description", "", "Description for the content of the 134N channel.");

    defaults_.setValue("reference_channel", "126", "The reference channel (126, 127N, 127C, 128N, 128C, 129N, 129C, 130N, 130C, 131N, 131C, 132N, 132C, 133N, 133C, 134N).");
    defaults_.setValidStrings("reference_channel", TMTSixteenPlexQuantitationMethod::channel_names_);

    defaults_.setValue("correction_matrix", std::vector<std::string>{
                                              "NA/NA / NA/NA / 0.31/9.09 / 0.02/0.32",
                                              "NA/NA / NA/0.78 / NA/9.41 / NA/0.33",
                                              "NA/NA / 0.93/NA / 0.35/8.63 / 0.01/0.27",
                                              "NA/0.00 / 0.82/0.65 / NA/8.13 / NA/0.26",
                                              "0.00/NA / 1.47/NA / 0.34/6.91 / 0.00/0.15",
                                              "0.00/0.00 / 1.46/1.28 / NA/6.86 / NA/0.15",
                                              "0.13/NA / 2.59/NA / 0.32/6.07 / 0.1/0.09",
                                              "0.13/0.00 / 2.41/0.27 / NA/5.58 / NA/0.10",
                                              "0.04/NA / 3.10/NA / 0.42/4.82 / 0.02/0.06",
                                              "0.03/0.00 / 2.78/0.63 / NA/4.57 / NA/0.12",
                                              "0.08/NA / 3.90/NA / 0.47/3.57 / 0.00/0.04",
                                              "0.15/0.01 / 3.58/0.72 / NA/1.80 / NA/0.00",
                                              "0.11/NA / 4.55/NA / 0.43/1.86 / 0.00/0.00",
                                              "0.07/0.01 / 3.14/0.73 / NA/3.40 / NA/0.03",
                                              "0.22/NA / 4.96/NA / 0.34/1.03 / 0.00/NA",
                                              "0.30/0.03 / 5.49/0.62 / NA/1.14 / NA/NA"
                                            },
                       "Correction matrix for isotope distributions in percent from the Thermo data sheet (see documentation);"
                       " Please provide 16 entries (rows), separated by comma, where each entry contains 8 values in the following format: <-2C13>/<-N15-C13>/<-C13>/<-N15>/<+N15>/<+C13>/<+N15+C13>/<+2C13> e.g. one row may look like this: 'NA/0.00  /  0.82/0.65  /  NA/8.13  /  NA/0.26'. You may use whitespaces at your leisure to ease reading.");

    defaultsToParam_();
}

void TMTSixteenPlexQuantitationMethod::updateMembers_()
{
    channels_[0].description = param_.getValue("channel_126_description").toString();
    channels_[1].description = param_.getValue("channel_127N_description").toString();
    channels_[2].description = param_.getValue("channel_127C_description").toString();
    channels_[3].description = param_.getValue("channel_128N_description").toString();
    channels_[4].description = param_.getValue("channel_128C_description").toString();
    channels_[5].description = param_.getValue("channel_129N_description").toString();
    channels_[6].description = param_.getValue("channel_129C_description").toString();
    channels_[7].description = param_.getValue("channel_130N_description").toString();
    channels_[8].description = param_.getValue("channel_130C_description").toString();
    channels_[9].description = param_.getValue("channel_131N_description").toString();
    channels_[10].description = param_.getValue("channel_131C_description").toString();
    channels_[11].description = param_.getValue("channel_132N_description").toString();
    channels_[12].description = param_.getValue("channel_132C_description").toString();
    channels_[13].description = param_.getValue("channel_133N_description").toString();
    channels_[14].description = param_.getValue("channel_133C_description").toString();
    channels_[15].description = param_.getValue("channel_134N_description").toString();


    // compute the index of the reference channel
    std::vector<std::string>::const_iterator t_it = std::find(TMTSixteenPlexQuantitationMethod::channel_names_.begin(),
                                                         TMTSixteenPlexQuantitationMethod::channel_names_.end(),
                                                         param_.getValue("reference_channel"));

    reference_channel_ = t_it - TMTSixteenPlexQuantitationMethod::channel_names_.begin();
}

TMTSixteenPlexQuantitationMethod::TMTSixteenPlexQuantitationMethod(const TMTSixteenPlexQuantitationMethod& other):
IsobaricQuantitationMethod(other)
{
    channels_.clear();
    channels_.insert(channels_.begin(), other.channels_.begin(), other.channels_.end());

    reference_channel_ = other.reference_channel_;
}

TMTSixteenPlexQuantitationMethod& TMTSixteenPlexQuantitationMethod::operator=(const TMTSixteenPlexQuantitationMethod& rhs)
= default;

const String& TMTSixteenPlexQuantitationMethod::getMethodName() const
{
    return TMTSixteenPlexQuantitationMethod::name_;
}

const IsobaricQuantitationMethod::IsobaricChannelList& TMTSixteenPlexQuantitationMethod::getChannelInformation() const
{
    return channels_;
}

Size TMTSixteenPlexQuantitationMethod::getNumberOfChannels() const
{
    return 16;
}

Matrix<double> TMTSixteenPlexQuantitationMethod::getIsotopeCorrectionMatrix() const
{
    StringList iso_correction = ListUtils::toStringList<std::string>(getParameters().getValue("correction_matrix"));
    return stringListToIsotopeCorrectionMatrix_(iso_correction);
}

Size TMTSixteenPlexQuantitationMethod::getReferenceChannel() const
{
    return reference_channel_;
}

} // namespace
