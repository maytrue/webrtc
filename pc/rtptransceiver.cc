/*
 *  Copyright 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "pc/rtptransceiver.h"

#include <string>

namespace webrtc {

RtpTransceiver::RtpTransceiver(cricket::MediaType media_type)
    : unified_plan_(false), media_type_(media_type) {
  RTC_DCHECK(media_type == cricket::MEDIA_TYPE_AUDIO ||
             media_type == cricket::MEDIA_TYPE_VIDEO);
}

RtpTransceiver::~RtpTransceiver() {
  Stop();
}

void RtpTransceiver::SetChannel(cricket::BaseChannel* channel) {
  if (channel) {
    RTC_DCHECK_EQ(media_type(), channel->media_type());
  }
  channel_ = channel;
  for (auto sender : senders_) {
    if (media_type() == cricket::MEDIA_TYPE_AUDIO) {
      static_cast<AudioRtpSender*>(sender->internal())
          ->SetChannel(static_cast<cricket::VoiceChannel*>(channel));
    } else {
      static_cast<VideoRtpSender*>(sender->internal())
          ->SetChannel(static_cast<cricket::VideoChannel*>(channel));
    }
  }
  for (auto receiver : receivers_) {
    if (!channel) {
      receiver->internal()->Stop();
    }
    if (media_type() == cricket::MEDIA_TYPE_AUDIO) {
      static_cast<AudioRtpReceiver*>(receiver->internal())
          ->SetChannel(static_cast<cricket::VoiceChannel*>(channel));
    } else {
      static_cast<VideoRtpReceiver*>(receiver->internal())
          ->SetChannel(static_cast<cricket::VideoChannel*>(channel));
    }
  }
}

void RtpTransceiver::AddSender(
    rtc::scoped_refptr<RtpSenderProxyWithInternal<RtpSenderInternal>> sender) {
  RTC_DCHECK(!unified_plan_);
  RTC_DCHECK(sender);
  RTC_DCHECK_EQ(media_type(), sender->internal()->media_type());
  RTC_DCHECK(std::find(senders_.begin(), senders_.end(), sender) ==
             senders_.end());
  senders_.push_back(sender);
}

bool RtpTransceiver::RemoveSender(RtpSenderInterface* sender) {
  RTC_DCHECK(!unified_plan_);
  if (sender) {
    RTC_DCHECK_EQ(media_type(), sender->media_type());
  }
  auto it = std::find(senders_.begin(), senders_.end(), sender);
  if (it == senders_.end()) {
    return false;
  }
  (*it)->internal()->Stop();
  senders_.erase(it);
  return true;
}

void RtpTransceiver::AddReceiver(
    rtc::scoped_refptr<RtpReceiverProxyWithInternal<RtpReceiverInternal>>
        receiver) {
  RTC_DCHECK(!unified_plan_);
  RTC_DCHECK(receiver);
  RTC_DCHECK_EQ(media_type(), receiver->internal()->media_type());
  RTC_DCHECK(std::find(receivers_.begin(), receivers_.end(), receiver) ==
             receivers_.end());
  receivers_.push_back(receiver);
}

bool RtpTransceiver::RemoveReceiver(RtpReceiverInterface* receiver) {
  RTC_DCHECK(!unified_plan_);
  if (receiver) {
    RTC_DCHECK_EQ(media_type(), receiver->media_type());
  }
  auto it = std::find(receivers_.begin(), receivers_.end(), receiver);
  if (it == receivers_.end()) {
    return false;
  }
  (*it)->internal()->Stop();
  receivers_.erase(it);
  return true;
}

rtc::Optional<std::string> RtpTransceiver::mid() const {
  return mid_;
}

rtc::scoped_refptr<RtpSenderInterface> RtpTransceiver::sender() const {
  RTC_DCHECK(unified_plan_);
  RTC_CHECK_EQ(1u, senders_.size());
  return senders_[0];
}

rtc::scoped_refptr<RtpReceiverInterface> RtpTransceiver::receiver() const {
  RTC_DCHECK(unified_plan_);
  RTC_CHECK_EQ(1u, receivers_.size());
  return receivers_[0];
}

bool RtpTransceiver::stopped() const {
  return stopped_;
}

RtpTransceiverDirection RtpTransceiver::direction() const {
  return direction_;
}

void RtpTransceiver::SetDirection(RtpTransceiverDirection new_direction) {
  // TODO(steveanton): This should fire OnNegotiationNeeded.
  direction_ = new_direction;
}

rtc::Optional<RtpTransceiverDirection> RtpTransceiver::current_direction()
    const {
  return current_direction_;
}

void RtpTransceiver::Stop() {
  for (auto sender : senders_) {
    sender->internal()->Stop();
  }
  for (auto receiver : receivers_) {
    receiver->internal()->Stop();
  }
  stopped_ = true;
}

void RtpTransceiver::SetCodecPreferences(
    rtc::ArrayView<RtpCodecCapability> codecs) {
  // TODO(steveanton): Implement this.
  RTC_NOTREACHED() << "Not implemented";
}

}  // namespace webrtc
