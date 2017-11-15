#pragma once

#include <memory>
#include <algorithm>
#include <cmath>

template <typename Key, typename Value=Key>
class AVLTree {
public:
    class Node {
    private:
        Key key_;
        Value value_;
        int height;
        std::unique_ptr<Node> left_, right_;
        Node(const Key& key) : key_(key), height(0) {}
        Node(const Key& key, const Value& value) : key_(key), value_(value), height(0) {}
        
    public:
        Node *left() {
            return left_.get();
        }
        Node *right() {
            return right_.get();
        }
        const Key& key() const {
            return key_;
        }
        const Value& value() const {
            return value_;
        }
        
        friend class AVLTree<Key, Value>;
    };
    
private:
    std::unique_ptr<Node> root_;
    int size_;
    
public:
    AVLTree() : size_(0) {}
    
    int findHeight(std::unique_ptr<Node>* node, int height)
    {
        if((*node)->right_ == nullptr && (*node)->left_== nullptr)
            return height;
        else if((*node)->right_ != nullptr && (*node)->left_ != nullptr)
        {
            height++;
            return std::max(findHeight(&(*node)->right_, height), findHeight(&(*node)->left_, height));
        }
        else if((*node)->right_ != nullptr)
        {
            height++;
            return findHeight(&(*node)->right_, height);
        }
        else
        {
            height++;
            return findHeight(&(*node)->left_, height);
        }
    }
    
    void updateHeights(std::unique_ptr<Node>* node)
    {
        if((*node)->right_ == nullptr && (*node)->left_== nullptr)
            return;
        else
        {
            (*node)->height = findHeight(&(*node), 0);
            if((*node)->right_ != nullptr)
                updateHeights(&(*node)->right_);
            if((*node)->left_ != nullptr)
                updateHeights(&(*node)->left_);
            return;
        }
    }
    
    
    void rightRotation (std::unique_ptr<Node>* parent, std::unique_ptr<Node>* node)
    {
        Node* parent_temp = parent->release();
        Node* swaparoo = (*node)->left_.release();
        parent = node;
        node->reset(swaparoo);
        (*parent)->right_.reset(parent_temp);
        
    }
    
    void leftRotation (std::unique_ptr<Node>* parent, std::unique_ptr<Node>* node)
    {
        Node* parent_temp = parent->release();
        Node* swaparoo = (*node)->right_.release();
        parent = node;
        node->reset(swaparoo);
        (*parent)->right_.reset(parent_temp);
    }
    
    void reblance(std::unique_ptr<Node>* node)
    {
        if((*node)->right_ == nullptr && (*node)->left_ == nullptr)
            return;
        int leftHeight = (*node)->left_ == nullptr ? 0 : (*node)->left_->height;
        int rightHeight = (*node)->right_ == nullptr ? 0 : (*node)->right_->height;
        int heightDiff = rightHeight - leftHeight;
        while (std::abs(heightDiff) > 2) { // find the node where rotation should occur
            node = (heightDiff > 0) ? (*node)->right_ : (*node)->left_;
        }
        if(heightDiff > 1) //right heavy
            {
                int innerLeftHeight = (*node)->right_->left_ == nullptr ? 0 : (*node)->right_->left_->height;
                int innerRightHeight = (*node)->right_->right_ == nullptr ? 0 : (*node)->right_->right_->height;
                int innerHeightDiff = innerRightHeight - innerLeftHeight;
                if(innerHeightDiff > 0)//outter heavy
                {
                    leftRotation(&(*node)->right_,&(*node)->right_->right_);
                    updateHeights(&root_);
                }
                else //inner heavy
                {
                    rightRotation(&(*node)->right_, &(*node)->right_->left_);
                    leftRotation(&(*node),&(*node)->right_);
                    updateHeights(&root_);
                }
            }
            else if(heightDiff < -1) //left heavy
            {
                int innerLeftHeight = (*node)->left_->left_ == nullptr ? 0 : (*node)->left_->left_->height;
                int innerRightHeight = (*node)->left_->right_ == nullptr ? 0 : (*node)->left_->right_->height;
                int innerHeightDiff = innerRightHeight - innerLeftHeight;
                if(innerHeightDiff < 0)//inner heavy
                {
                    leftRotation(&(*node)->left_,&(*node)->left_->right_);
                    rightRotation(&(*node),&(*node)->left_);
                    updateHeights(&root_);
                }
                else //outter heavy
                {
                    rightRotation(&(*node)->left_,&(*node)->left_->left_);
                    updateHeights(&root_);
                }

            }
    }
    
    
    
    // TODO: Add code to update node heights and do rebalancing...
    Value& operator[](const Key& key) {
        // Try to find the node with the value we want:
        std::unique_ptr<Node> *cur;
        for (cur = &root_;
             cur->get() != nullptr;
             cur = key < (*cur)->key_ ? &(*cur)->left_ : &(*cur)->right_) {
            if (key == (*cur)->key_) {
                return (*cur)->value_;
            }
        }
        // If we didn't find it, insert a new node with that key:
        // (This is the same behaviour as an std::map.)
        cur->reset(new Node(key));
        ++size_;
        updateHeights(&root_);
        reblance(&root_);
        std::cout << "The height is " << findHeight(&root_, 1) << std::endl;
        return (*cur)->value_;
    }
    
    int size() {
        return size_;
    }
    
    Node *root() {
        return root_.get();
    }
};
