#include <memory>
#include <stdexcept>
#include <vector>


struct BinaryTreeNode {
    using Self = BinaryTreeNode;
    using ParentPtr = Self*;
    using ChildPtr = std::unique_ptr<Self>;

    int data;
    ParentPtr parent;
    struct Children {
        ChildPtr l = nullptr;
        ChildPtr r = nullptr;
    } ch;

    // constructors
    static ChildPtr create_leaf(int data) {
        return std::make_unique<Self>(data, nullptr, Children{});
    }
    static ChildPtr create(int data, Children&& children) {
        auto self = std::make_unique<Self>(data, nullptr, std::move(children));
        if (auto& l = self->ch.l) 
            l->parent = self.get();
        if (auto& r = self->ch.r)
            r->parent = self.get();
        return self;
    }
};

// traverses the tree in-order (LNR)
class BinaryTreeIterator {
public:
    using Self = BinaryTreeIterator;
    using Node = BinaryTreeNode;
private:
    // ...
public:
    // get an iterator to the next element
    Self next() const {
        if (!*this)
            throw std::runtime_error("Can't increment an invalid iterator");
        // ...
    }

    // get an iterator to the previous element
    Self prev() const {
        if (!*this)
            throw std::runtime_error("Can't decrement an invalid iterator");
        // ...
    }

    // check if iterator is valid
    operator bool() const {
        // ...
    }

    // access the underlying element
    decltype(Node::data)& operator*() {
        if (!*this)
            throw std::runtime_error("Can't dereference an invalid iterator");
        // ...
    }
};

// see bonus #1
// traverses the tree in-order + checks if node was deleted
class SafeBinaryTreeIterator {
public:
    using Self = SafeBinaryTreeIterator;
    using Unsafe = BinaryTreeIterator;
    using Node = BinaryTreeNode;
private:
    // ...
public:
    // get an unsafe iterator
    Unsafe to_unsafe() const {
        if (!*this)
            throw std::runtime_error("Can't convert invalid safe iterator to unsafe one");
        // ...
    }

    // get an iterator to the next element
    Self next() const {
        if (!*this)
            throw std::runtime_error("Can't increment an invalid iterator");
        // ...
    }

    // get an iterator to the previous element
    Self prev() const {
        if (!*this)
            throw std::runtime_error("Can't decrement an invalid iterator");
        // ...
    }

    // check if iterator is valid
    operator bool() const {
        // ...
    }

    // access the underlying element
    decltype(Node::data)& operator*() {
        if (!*this)
            throw std::runtime_error("Can't dereference an invalid iterator");
        // ...
    }
};

class BinaryTree {
public:
    using Self = BinaryTree;
    using Node = BinaryTreeNode;
    using Iterator = BinaryTreeIterator;
    using SafeIterator = SafeBinaryTreeIterator;
private:
    // ...
public:
    static Self from(Node::ChildPtr&& root) {
        // ...
    }

    // iterator to the first element of a tree
    Iterator front_iter() {
        // ...
    }

    // iterator to the last element of a tree
    Iterator back_iter() {
        // ...
    }

    // see bonus #1
    SafeIterator safe_front_iter() {
        // ...
    }
    SafeIterator safe_back_iter() {
        // ...
    }
    void erase(Iterator it) {
        // ...
    }
    void erase(SafeIterator it) {
        erase(it.to_unsafe());
    }
};


bool assert_forward_eq(BinaryTreeIterator it, const std::vector<int>& seq) {
    for (size_t i = 0; i < seq.size(); ++i, it = it.next()) {
        if (!it || *it != seq[i])
            return false;
    }
    return !it;
}

bool assert_backward_eq(BinaryTreeIterator it, const std::vector<int>& seq) {
    for (size_t i = 0; i < seq.size(); ++i, it = it.prev()) {
        if (!it || *it != seq[i])
            return false;
    }
    return !it;
}

bool print_test_msg(bool pass, const char* name) {
    std::printf(pass ? "\x1b[32m`%s` pass\x1b[0m\n" : "\x1b[31m`%s` didn't pass\x1b[0m\n", name);
    return pass;
}

void run_suite(const std::vector<bool(*)()>& tests) {
    for (auto test : tests) {
        test();
    }
}


bool test_forward_empty() {
    auto tree = BinaryTree::from(nullptr);
    return print_test_msg(!tree.front_iter(), "test_empty");
}

bool test_forward_bamboo_left() {
    auto tree = BinaryTree::from(
        BinaryTreeNode::create(4, {
            BinaryTreeNode::create(3, {
                BinaryTreeNode::create(2, {
                    BinaryTreeNode::create(1, {
                        BinaryTreeNode::create_leaf(0)
                    })
                })
            })
        })
    );
    return print_test_msg(assert_forward_eq(tree.front_iter(), { 0, 1, 2, 3, 4 }), "test_bamboo_left");
}

bool test_forward_bamboo_right() {
    auto tree = BinaryTree::from(
        BinaryTreeNode::create(0, {
            .r = BinaryTreeNode::create(1, {
                .r = BinaryTreeNode::create(2, {
                    .r = BinaryTreeNode::create(3, {
                        .r = BinaryTreeNode::create_leaf(4)
                    })
                })
            })
        })
    );
    return print_test_msg(assert_forward_eq(tree.front_iter(), { 0, 1, 2, 3, 4 }), "test_bamboo_right");
}

bool test_forward_full() {
    auto tree = BinaryTree::from(
        BinaryTreeNode::create(3, {
            BinaryTreeNode::create(1, {
                BinaryTreeNode::create_leaf(0),
                BinaryTreeNode::create_leaf(2),
            }),
            BinaryTreeNode::create(5, {
                BinaryTreeNode::create_leaf(4),
                BinaryTreeNode::create_leaf(6),
            }),
        })
    );
    return print_test_msg(assert_forward_eq(tree.front_iter(), { 0, 1, 2, 3, 4, 5, 6 }), "test_full");
}

void forward_tests() {
    std::printf("Running `forward_tests`:\n");
    run_suite({ 
        test_forward_empty, test_forward_bamboo_left,
        test_forward_bamboo_right, test_forward_full
    });
    std::printf("\n");
}


bool test_backward_empty() {
    auto tree = BinaryTree::from(nullptr);
    return print_test_msg(!tree.back_iter(), "test_empty");
}

bool test_backward_bamboo_left() {
    auto tree = BinaryTree::from(
        BinaryTreeNode::create(0, {
            BinaryTreeNode::create(1, {
                BinaryTreeNode::create(2, {
                    BinaryTreeNode::create(3, {
                        BinaryTreeNode::create_leaf(4)
                    })
                })
            })
        })
    );
    return print_test_msg(assert_backward_eq(tree.back_iter(), { 0, 1, 2, 3, 4 }), "test_bamboo_left");
}

bool test_backward_bamboo_right() {
    auto tree = BinaryTree::from(
        BinaryTreeNode::create(4, {
            .r = BinaryTreeNode::create(3, {
                .r = BinaryTreeNode::create(2, {
                    .r = BinaryTreeNode::create(1, {
                        .r = BinaryTreeNode::create_leaf(0)
                    })
                })
            })
        })
    );
    return print_test_msg(assert_backward_eq(tree.back_iter(), { 0, 1, 2, 3, 4 }), "test_bamboo_right");
}

bool test_backward_full() {
    auto tree = BinaryTree::from(
        BinaryTreeNode::create(3, {
            BinaryTreeNode::create(5, {
                BinaryTreeNode::create_leaf(6),
                BinaryTreeNode::create_leaf(4),
            }),
            BinaryTreeNode::create(1, {
                BinaryTreeNode::create_leaf(2),
                BinaryTreeNode::create_leaf(0),
            }),
        })
    );
    return print_test_msg(assert_backward_eq(tree.back_iter(), { 0, 1, 2, 3, 4, 5, 6 }), "test_full");
}

void backward_tests() {
    std::printf("Running `backward_tests`:\n");
    run_suite({ 
        test_backward_empty, test_backward_bamboo_left,
        test_backward_bamboo_right, test_backward_full
    });
    std::printf("\n");
}


bool test_invalidate_delete_root() {
    auto tree = BinaryTree::from(
        BinaryTreeNode::create(3, {
            BinaryTreeNode::create(1, {
                BinaryTreeNode::create_leaf(0),
                BinaryTreeNode::create_leaf(2),
            }),
            BinaryTreeNode::create_leaf(4),
        })
    );
    auto it0 = tree.safe_front_iter();
    auto it1 = it0.next();
    auto it2 = it1.next();
    auto it3 = it2.next();
    auto it4 = it3.next();
    tree.erase(it3);
    return print_test_msg(!it0 && !it1 && !it2 && !it3 && !it4, "test_delete_root");
}

bool test_invalidate_all() {
    auto tree = BinaryTree::from(
        BinaryTreeNode::create(3, {
            BinaryTreeNode::create(1, {
                BinaryTreeNode::create_leaf(0),
                BinaryTreeNode::create_leaf(2),
            }),
            BinaryTreeNode::create_leaf(4),
        })
    );
    auto it0 = tree.safe_front_iter();
    auto it1 = it0.next();
    auto it2 = it1.next();
    auto it3 = it2.next();
    auto it4 = it3.next();
    tree.erase(it1);
    return print_test_msg(!it0 && !it1 && !it2 && !it3 && !it4, "test_invalidate_all");
}

bool test_invalidate_affected() {
    auto tree = BinaryTree::from(
        BinaryTreeNode::create(3, {
            BinaryTreeNode::create(1, {
                BinaryTreeNode::create_leaf(0),
                BinaryTreeNode::create_leaf(2),
            }),
            BinaryTreeNode::create_leaf(4),
        })
    );
    auto it0 = tree.safe_front_iter();
    auto it1 = it0.next();
    auto it2 = it1.next();
    auto it3 = it2.next();
    auto it4 = it3.next();
    tree.erase(it1);
    return print_test_msg(!it0 && !it1 && !it2 && it3 && it4, "test_invalidate_affected");
}

void invalidation_tests() {
    std::printf("Running `invalidation_tests`:\n");
    run_suite({ 
        test_invalidate_delete_root,
        // test_invalidate_all,
        test_invalidate_affected,
    });
    std::printf("\n");
}

/*

  Bonuses:
  
  1. If we were able to delete subtrees what invalidation guarantees does current implementation provide? 
  1.1. Lets design a 'safe' iteration of a tree, which invalidates:
    a) All iterators after delete
    b) Only affected iterators after delete
  1.2. Can we make such iteration an optional thing and are there any slowdowns (runtime-wise) 
       if user wills to avoid it? Can we make it better?

*/

int main(int, char**) {
    forward_tests();
    backward_tests();
    // invalidation_tests();
}
