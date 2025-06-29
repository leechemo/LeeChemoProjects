<template>
  <div class="container py-4">
    <h1 class="text-center mb-4 text-primary">Meal Plan</h1>

    <div v-if="loading" class="text-center py-5">
      <div class="spinner-border text-primary mb-3" role="status">
        <span class="visually-hidden">Loading...</span>
      </div>
      <p class="text-muted">Loading meal plan...</p>
    </div>

    <div v-else-if="mealPlan.length > 0">
      <div class="mb-3 text-end">
        <button class="btn btn-outline-danger" @click="clearMealPlan">
          <i class="bi bi-trash"></i> Clear Meal Plan
        </button>
      </div>

      <ol class="list-group list-group-numbered">
        <li v-for="(item, index) in sortedMealPlan" :key="item.recipeID" class="list-group-item d-flex justify-content-between align-items-center">
          <div class="d-flex align-items-center">
            <img :src="item.image" alt="recipe image" class="me-3 rounded" style="width: 80px; height: 80px; object-fit: cover;">
            <div>
              <h5>{{ item.title }}</h5>
              <div class="small text-muted">
                <span v-if="item.vegan" class="badge bg-success me-1"><i class="bi bi-leaf"></i> Vegan</span>
                <span v-if="item.vegetarian" class="badge bg-info me-1"><i class="bi bi-flower1"></i> Vegetarian</span>
                <span v-if="item.glutenFree" class="badge bg-warning"><i class="bi bi-shield-check"></i> Gluten-Free</span>
              </div>

              <div class="small text-muted">Ready in {{ item.readyInMinutes }} minutes</div>
              <div class="mt-2">
                <div class="progress" style="height: 20px;">
                  <div 
                    class="progress-bar" 
                    role="progressbar" 
                    :style="{ width: getProgressPercentage(item) + '%' }"
                    :aria-valuenow="getProgressPercentage(item)"
                    aria-valuemin="0" aria-valuemax="100">
                    {{ getProgressPercentage(item) }}%
                  </div>
                </div>
              </div>
            </div>
          </div>

          <div class="btn-group">
            <button class="btn btn-outline-secondary" @click="moveUp(item)" :disabled="index === 0">
              <i class="bi bi-arrow-up"></i>
            </button>
            <button class="btn btn-outline-secondary" @click="moveDown(item)" :disabled="index === mealPlan.length - 1">
              <i class="bi bi-arrow-down"></i>
            </button>
            <button class="btn btn-outline-danger" @click="removeFromMealPlan(item)">
              <i class="bi bi-x"></i>
            </button>
          </div>
        </li>
      </ol>
    </div>

    <div v-else class="text-center py-5">
      <h4 class="text-muted">No recipes in your meal plan yet</h4>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
import store from '@/store';

export default {
  name: 'MealPlanPage',
  data() {
    return {
      loading: true,
      mealPlan: []
    };
  },

  created() {
    this.loadMealPlan();
  },

  computed: {
    sortedMealPlan() {
      return [...this.mealPlan].sort((a, b) => a.orderIndex - b.orderIndex);
    }
  },

  methods: {
    async loadMealPlan() {
      this.loading = true;
      try {
        const response = await axios.get(`${store.server_domain}/user/meal-plan`, { withCredentials: true });
        this.mealPlan = response.data;
      } catch (error) {
        console.error('Error loading meal plan:', error);
      } finally {
        this.loading = false;
      }
    },

    async clearMealPlan() {
      if (!confirm('Are you sure you want to clear the meal plan?')) return;
      try {
        await axios.delete(`${store.server_domain}/user/meal-plan`, { withCredentials: true });
        this.mealPlan = [];
      } catch (error) {
        console.error('Error clearing meal plan:', error);
      }
    },

    async removeFromMealPlan(item) {
      try {
        await axios.delete(`${store.server_domain}/user/meal-plan/${item.recipeID}`, { withCredentials: true });
        this.loadMealPlan();
      } catch (error) {
        console.error('Error removing recipe from meal plan:', error);
      }
    },

    async moveUp(item) {
      if (item.orderIndex === 1) return; 
      const newOrder = item.orderIndex - 1;
      await this.updateOrder(item.recipeID, newOrder);
    },

    async moveDown(item) {
      if (item.orderIndex === this.mealPlan.length) return;
      const newOrder = item.orderIndex + 1;
      await this.updateOrder(item.recipeID, newOrder);
    },


    async updateOrder(recipeID, newOrder) {
      try {
        await axios.patch(`${store.server_domain}/user/meal-plan/${recipeID}`, { orderIndex: newOrder }, { withCredentials: true });
        this.loadMealPlan();
      } catch (error) {
        console.error('Error updating order:', error);
      }
    },

    getProgressPercentage(item) {
    if (!item.progress || !item.progress.completedSteps) {
        return 0;
    }
    const total = item.totalSteps;  
    const completed = item.progress.completedSteps.length;
    return total > 0 ? Math.round((completed / total) * 100) : 0;
    }

  }
};

</script>

<style scoped>
.progress-bar {
  font-weight: 500;
}
.list-group-item {
  border-radius: 10px;
  margin-bottom: 10px;
  box-shadow: 0 0 5px rgba(0,0,0,0.1);
}
</style>
