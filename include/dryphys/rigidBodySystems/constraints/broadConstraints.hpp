// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: broadConstraints.hpp
// Author: crdrisko
// Date: 12/28/2024-06:56:29
// Description:

#ifndef BROADCOLLISION_HPP
#define BROADCOLLISION_HPP

#include "dryphys/math/vector.hpp"
#include "dryphys/types/rigidBody.hpp"
#include "dryphys/utilities/config.hpp"

namespace DryPhys
{
    struct BoundingBox
    {
        Vector3D center_ {};
        Vector3D halfSize_ {};

        //! Constructors
        BoundingBox(const BoundingBox& one, const BoundingBox& two);
        constexpr BoundingBox(const Vector3D& center, const Vector3D& halfSize) : center_ {center}, halfSize_ {halfSize} {}

        constexpr bool overlaps(const BoundingBox* other) const
        {
            real distanceSquared = (center_ - other->center_).magnitudeSquared();

            Vector3D distances {distanceSquared, distanceSquared, distanceSquared};

            return distances < (halfSize_ + other->halfSize_) * (halfSize_ + other->halfSize_);
        }

        // real getGrowth(const BoundingBox&) const {}

        constexpr real getSize() const { return halfSize_.x * halfSize_.y * halfSize_.z * static_cast<real>(8); }
    };

    struct BoundingSphere
    {
        Vector3D center_ {};
        real radius_ {};

        //! Constructors
        BoundingSphere(const BoundingSphere& one, const BoundingSphere& two);
        constexpr BoundingSphere(const Vector3D& center, real radius) : center_ {center}, radius_ {radius} {}

        real getGrowth(const BoundingSphere& other) const;

        constexpr bool overlaps(const BoundingSphere* other) const
        {
            real distanceSquared = (center_ - other->center_).magnitudeSquared();

            return distanceSquared < (radius_ + other->radius_) * (radius_ + other->radius_);
        }

        constexpr real getSize() const { return static_cast<real>(1.333333) * Constants::pi * radius_ * radius_ * radius_; }
    };

    struct PotentialContact
    {
        RigidBody* bodies_[2];
    };

    template<typename BoundingVolumeClass>
    class BVHNode
    {
    public:
        BVHNode* children_[2];
        BoundingVolumeClass volume_;
        RigidBody* body_;
        BVHNode* parent_;

        constexpr BVHNode(BVHNode* parent, const BoundingVolumeClass& volume, RigidBody* body = nullptr)
            : parent_ {parent}, volume_ {volume}, body_ {body}
        {
            children_[0] = children_[1] = nullptr;
        }

        ~BVHNode()
        {
            if (parent_)
            {
                BVHNode<BoundingVolumeClass>* sibling;

                if (parent_->children_[0] == this)
                    sibling = parent_->children_[1];
                else
                    sibling = parent_->children_[0];

                parent_->volume_      = sibling->volume_;
                parent_->body_        = sibling->body_;
                parent_->children_[0] = sibling->children_[0];
                parent_->children_[1] = sibling->children_[1];

                sibling->parent_      = nullptr;
                sibling->body_        = nullptr;
                sibling->children_[0] = nullptr;
                sibling->children_[1] = nullptr;

                delete sibling;

                parent_->recalculateBoundingVolume();
            }

            if (children_[0])
            {
                children_[0]->parent_ = nullptr;
                delete children_[0];
            }

            if (children_[1])
            {
                children_[1]->parent_ = nullptr;
                delete children_[1];
            }
        }

        constexpr bool isLeaf() const { return (body_ != nullptr); }

        constexpr unsigned getPotentialContacts(PotentialContact* contacts, unsigned limit) const
        {
            if (isLeaf() || limit == 0)
                return 0;

            return children_[0]->getPotentialContactsWith(children_[1], contacts, limit);
        }

        void insert(RigidBody* newBody, const BoundingVolumeClass& newVolume)
        {
            if (isLeaf())
            {
                children_[0] = new BVHNode<BoundingVolumeClass>(this, volume_, body_);
                children_[1] = new BVHNode<BoundingVolumeClass>(this, newVolume, newBody);

                this->body_ = nullptr;

                recalculateBoundingVolume();
            }
            else
            {
                if (children_[0]->volume_.getGrowth(newVolume) < children_[1]->volume_.getGrowth(newVolume))
                {
                    children_[0]->insert(newBody, newVolume);
                }
                else
                {
                    children_[1]->insert(newBody, newVolume);
                }
            }
        }

    protected:
        constexpr bool overlaps(const BVHNode<BoundingVolumeClass>* other) const
        {
            return volume_->overlaps(other->volume_);
        }

        constexpr unsigned getPotentialContactsWith(
            const BVHNode<BoundingVolumeClass>* other, PotentialContact* contacts, unsigned limit) const
        {
            if (!overlaps(other) || limit == 0)
                return 0;

            if (isLeaf() && other->isLeaf())
            {
                contacts->bodies_[0] = body_;
                contacts->bodies_[1] = other->body_;

                return 1;
            }

            if (other->isLeaf() || (!isLeaf() && volume_->getSize() >= other->volume_->getSize()))
            {
                // Recurse into ourself
                unsigned count = children_[0]->getPotentialContactsWith(other, contacts, limit);

                // Check if we have enough slots to do the other side too
                if (limit > count)
                {
                    return count + children_[1]->getPotentialContactsWith(other, contacts + count, limit - count);
                }
                else
                {
                    return count;
                }
            }
            else
            {
                // Recurse into the other node
                unsigned count = getPotentialContactsWith(other->children_[0], contacts, limit);

                // Check if we have enough slots to do the other side too
                if (limit > count)
                {
                    return count + getPotentialContactsWith(other->children_[1], contacts + count, limit - count);
                }
                else
                {
                    return count;
                }
            }
        }

        constexpr void recalculateBoundingVolume()
        {
            if (isLeaf())
                return;

            volume_ = BoundingVolumeClass(children_[0], children_[1]);

            // Recurse up the tree
            if (parent_)
                parent_->recalculateBoundingVolume(true);
        }
    };
}   // namespace DryPhys

#endif
