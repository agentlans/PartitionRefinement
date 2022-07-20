"""
   Copyright 2022 Alan Tseng

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
"""

"""
member[x] = set of the set containing x
place[x] = index of x
begin[s] = the beginning of set s
end[s] = the end index of set s

split[s] = id of s's split set (use as temporary storage)
"""

class PartitionRefinement:
    def __init__(self, lst):
        "Initializes a set containing the elements of lst."
        self.member = dict()
        self.place = dict()
        self.begin = dict()
        self.end = dict()
        # Optional vector to store data if we want to see the sets
        self.data = list(lst)
        # Fill the items
        for i, x in enumerate(lst):
            self.member[x] = 0
            self.place[x] = i
        self.begin[0] = 0
        self.end[0] = len(lst)
        # ID of a new set
        self.next_split = 1
    def as_list(self):
        "Returns the partitions as a list of lists."
        out = []
        for s in self.begin:
            subset = [self.data[i] \
                for i in range(self.begin[s], self.end[s])]
            out.append(subset)
        return out
    def refine(self, lst):
        """Refines the partitions using the elements from lst.
        For each set S, S will be replaced with (S intersect lst)
        and (S \ lst)."""
        split = dict()
        for x in lst:
            s = self.member[x]  # Find item's set
            ss = self.next_split
            if s in split:
                # Find the split set containing x
                ss = split[s]
            else:
                # Create a new split
                self.next_split += 1
                ss += 1
                split[s] = ss
                self.begin[ss] = self.begin[s]
                self.end[ss] = self.begin[s]
            # Add item to split set
            self.member[x] = ss
            # Put x at the end of the split set.
            # Swap the indices of data items
            a = self.place[x]
            b = self.end[ss]
            self.place[self.data[b]] = a
            self.place[self.data[a]] = b
            # Swap the data
            tmp = self.data[a]
            self.data[a] = self.data[b]
            self.data[b] = tmp
            # Split set's size increased by 1
            self.end[ss] += 1
        # Update the bounds of sets that have been split off
        for s in split:
            self.begin[s] = self.end[split[s]]

if __name__ == "__main__":
    # Example
    pr = PartitionRefinement(range(10))
    pr.refine([2,3,5,7])
    pr.refine([0,2,4,6,8])
    print(pr.as_list())
